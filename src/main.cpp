#include <aws/crt/UUID.h>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <mutex>
#include <vector>
#include <chrono>
#include <omp.h>

#include "CommandLineUtils.h"
#include "Communication.h"
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

    omp_set_nested(1);
    cout << "\nStarting.." << endl;
    cout << "Processors " << omp_get_num_procs() << endl;

    #pragma omp parallel sections default(none) shared(cout, connection, clientId, topic) num_threads(2)
    {
        #pragma omp section
        {
            #pragma omp critical
            cout << "* Starting VehicleControl in Thread: " << omp_get_thread_num() << endl;

            VehicleControl();
        }
        #pragma omp section
        {
            #pragma omp critical
            cout << "* Starting AWS IoT communication in Thread: " << omp_get_thread_num() << endl;

            ThreadSafeQueue threadSafeQueue;

            // open the connection to AWS IoT (MQTT) and listen for messages
            Communication(
                    threadSafeQueue,
                    connection,
                    clientId,
                    topic
            );

            vector<Document> vehicles;

            while (true) {
                string message = threadSafeQueue.front();  // blocking
                threadSafeQueue.pop();

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

                cout << vehicles.size() << " vehicles in the array" << endl;
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

vehicle["x"].SetDouble(doc["x"].GetDouble());
vehicle["y"].SetDouble(doc["y"].GetDouble());
vehicle["isBraking"].SetBool(doc["isBraking"].GetBool());
vehicle["speed"].SetDouble(doc["speed"].GetDouble());
vehicle["direction"].SetInt(doc["direction"].GetInt());
vehicle["joined"].SetUint(doc["joined"].GetUint());
vehicle["received"].SetUint(doc["received"].GetUint());
 */
