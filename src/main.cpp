#include <aws/crt/UUID.h>
//#include "rapidjson/document.h"
//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"

#include <iostream>
#include <mutex>

#include "CommandLineUtils.h"
#include "Communication.h"

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

    ThreadSafeQueue threadSafeQueue;

    Communication(
        threadSafeQueue,
        connection,
        clientId,
        topic
    );

    while(true) {
//        Document doc;
//        doc.Parse(message.c_str());
        cout << "msg: " << threadSafeQueue.front();
        threadSafeQueue.pop();
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
