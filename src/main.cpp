#include <aws/crt/UUID.h>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <mutex>
#include <vector>
#include <chrono>
#include <omp.h>

#include "CommandLineUtils.h"
#include "Communication.h"
#include "ThreadSafeQueue.h"
#include "VehicleControl.h"

using namespace Aws::Crt;
using namespace rapidjson;
using namespace std;

int main(int argc, char *argv[]) {
    ApiHandle apiHandle;

    /*********************** Parse Arguments ***************************/
    Utils::CommandLineUtils cmdUtils = Utils::CommandLineUtils();
    cmdUtils.RegisterProgramName("basic_pub_sub");
    cmdUtils.AddCommonMQTTCommands();
    cmdUtils.RegisterCommand("key", "<path>", "Path to your key in PEM format.");
    cmdUtils.RegisterCommand("cert", "<path>", "Path to your client certificate in PEM format.");
    cmdUtils.AddCommonProxyCommands();
    cmdUtils.AddCommonTopicMessageCommands();
    cmdUtils.RegisterCommand("client_id", "<str>", "Client id to use (optional, default='test-*')");
    cmdUtils.RegisterCommand("count", "<int>", "The number of messages to send (optional, default='10')");
    cmdUtils.RegisterCommand("port_override", "<int>", "The port override to use when connecting (optional)");
    cmdUtils.AddLoggingCommands();
    const char **const_argv = (const char **)argv;
    cmdUtils.SendArguments(const_argv, const_argv + argc);
    cmdUtils.StartLoggingBasedOnCommand(&apiHandle);

    String topic = cmdUtils.GetCommandOrDefault("topic", "test/topic");
    String clientId = cmdUtils.GetCommandOrDefault("client_id", String("test-") + Aws::Crt::UUID().ToString());

    auto connection = cmdUtils.BuildMQTTConnection();

    // initialise this vehicles model
    String vehicleLabel = UUID().ToString();
    Document vehicleModel;
    vehicleModel.SetObject();
    Value id(vehicleLabel.c_str(), vehicleModel.GetAllocator());
    Value x(0.0);
    Value y(0.0);
    Value isBraking(false);
    Value speed(0.0);
    Value direction(0);
    Value joined(0);
    vehicleModel.AddMember("id", id, vehicleModel.GetAllocator());
    vehicleModel.AddMember("x", x, vehicleModel.GetAllocator());
    vehicleModel.AddMember("y", y, vehicleModel.GetAllocator());
    vehicleModel.AddMember("isBraking", isBraking, vehicleModel.GetAllocator());
    vehicleModel.AddMember("speed", speed, vehicleModel.GetAllocator());
    vehicleModel.AddMember("direction", direction, vehicleModel.GetAllocator());
    vehicleModel.AddMember("joined", joined, vehicleModel.GetAllocator());

    omp_set_nested(1);
    cout << "\nStarting.." << endl;
    cout << "Processors " << omp_get_num_procs() << endl;

    ThreadSafeQueue threadSafeQueue;

    #pragma omp parallel sections default(none) shared(cout, threadSafeQueue, connection, clientId, topic, vehicleModel) num_threads(2)
    {
        #pragma omp section
        {
            #pragma omp critical
            cout << "* Starting VehicleControl in Thread: " << omp_get_thread_num() << endl;

            auto vehicleControl = VehicleControl(vehicleModel, threadSafeQueue);
        }
        #pragma omp section
        {
            #pragma omp critical
            cout << "* Starting AWS IoT communication in Thread: " << omp_get_thread_num() << endl;

            // open the connection to AWS IoT (MQTT)
            // event based messages will be returned in the treadSafeQueued
            Communication communication = Communication(
                    threadSafeQueue,
                    connection,
                    clientId,
                    topic
            );

            vector<Document> vehicles;

            while (true) {
                // block until AWS message received
                string message = threadSafeQueue.front();
                threadSafeQueue.pop();

                if (strcmp(message.c_str(), "update") == 0) {
                    // convert the vehicle model json to a string
                    StringBuffer buffer;
                    Writer<StringBuffer> writer(buffer);
                    #pragma omp critical
                    vehicleModel.Accept(writer);

                    // publish to the AWS channel
                    communication.publish(buffer.GetString());
                    continue;
                }

                // parse string->json
                Document doc;
                doc.Parse(message.c_str());

                // add received timestamp
                // TODO: Using server time here is preferred as it will cause critical issues if a machines time is wrong
                auto timestamp = chrono::duration_cast<chrono::seconds>(
                        chrono::system_clock::now().time_since_epoch()).count();
                doc.AddMember("received", timestamp, doc.GetAllocator());

                // existing vehicle, update the record
                bool updated = false;
                for (auto &vehicle: vehicles) {
                    if (strcmp(vehicle["id"].GetString(), doc["id"].GetString()) == 0) {
                        vehicle.Swap(doc);
                        updated = true;
                        break;
                    }
                }
                // new vehicle, add it
                if (!updated) {
                    vehicles.push_back(std::move(doc));
                }
            }
        }
    }
}


/*  SAMPLE MESSAGE
{
    "id": string,
    "x": double,
    "y": double,
    "isBraking": bool,
    "speed": double,
    "direction": int,
    "joined": time_t,
    "received": time_t,
}

{
    "id": "123",
    "x": 10.5,
    "y": 20.0,
    "isBraking": false,
    "speed": 50.0,
    "direction": 90,
    "joined": 1644723600,
    "received": 1644723650
}
 */
