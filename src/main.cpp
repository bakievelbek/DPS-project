#include <aws/crt/UUID.h>
#include <chrono>
#include <mutex>
#include <thread>

#include "CommandLineUtils.h"
#include "Communication.h"

using namespace Aws::Crt;
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

    Communication communication(
        connection,
        clientId,
        topic
    );

    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        communication.publish("something");
    }
}
