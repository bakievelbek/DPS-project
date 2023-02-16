#ifndef DPS_PROJECT_COMMUNICATION_H
#define DPS_PROJECT_COMMUNICATION_H

#include <aws/crt/Api.h>
#include <aws/crt/StlAllocator.h>
#include <aws/crt/auth/Credentials.h>
#include <aws/crt/io/TlsOptions.h>
#include <aws/iot/MqttClient.h>

#include "ThreadSafeQueue.h"

#include <thread>

using namespace Aws::Crt;


class Communication {
public:
    explicit Communication(
        ThreadSafeQueue threadSafeQueue,
        std::shared_ptr<Mqtt::MqttConnection> connection,
        String clientId,
        String topic
    );
    void publish(const char *message);
};


#endif //DPS_PROJECT_COMMUNICATION_H
