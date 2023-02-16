#include "Communication.h"

std::promise<bool> connectionCompletedPromise;
std::promise<void> connectionClosedPromise;

Communication::Communication(std::shared_ptr<Mqtt::MqttConnection> connection, String clientId, String topic) {
    auto onConnectionCompleted = [&](Mqtt::MqttConnection &, int errorCode, Mqtt::ReturnCode returnCode, bool) {
        if (errorCode) {
            fprintf(stdout, "Connection failed with error %s\n", ErrorDebugString(errorCode));
            connectionCompletedPromise.set_value(false);
        }
        else {
            fprintf(stdout, "Connection completed with return code %d\n", returnCode);
            connectionCompletedPromise.set_value(true);
        }
    };

    auto onInterrupted = [&](Mqtt::MqttConnection &, int error) {
        fprintf(stdout, "Connection interrupted with error %s\n", ErrorDebugString(error));
    };

    auto onResumed = [&](Mqtt::MqttConnection &, Mqtt::ReturnCode, bool) {
        fprintf(stdout, "Connection resumed\n");
    };

    auto onDisconnect = [&](Mqtt::MqttConnection &) {
        {
            fprintf(stdout, "Disconnected\n");
            connectionClosedPromise.set_value();
        }
    };

    connection->OnConnectionCompleted = std::move(onConnectionCompleted);
    connection->OnDisconnect = std::move(onDisconnect);
    connection->OnConnectionInterrupted = std::move(onInterrupted);
    connection->OnConnectionResumed = std::move(onResumed);

    // connect
    fprintf(stdout, "Connecting...\n");
    if (!connection->Connect(clientId.c_str(), false, 1000)) {
        fprintf(stderr, "MQTT Connection failed with error %s\n", ErrorDebugString(connection->LastError()));
        exit(-1);
    }

    if (connectionCompletedPromise.get_future().get()) {
//        std::mutex receiveMutex;
//        std::condition_variable receiveSignal;

        auto onMessage = [&](Mqtt::MqttConnection &,
            const String &topic,
            const ByteBuf &byteBuf,
            bool /*dup*/,
            Mqtt::QOS /*qos*/,
            bool /*retain*/
        ) {
            {
//                std::lock_guard<std::mutex> lock(receiveMutex);
                fwrite(byteBuf.buffer, 1, byteBuf.len, stdout);
                fprintf(stdout, "Received message on topic %s\n", topic.c_str());
            }

//            receiveSignal.notify_all();
        };

        std::promise<void> subscribeFinishedPromise;
        auto onSubAck =
                [&](Mqtt::MqttConnection &, uint16_t packetId, const String &topic, Mqtt::QOS QoS, int errorCode) {
                    if (errorCode) {
                        fprintf(stderr, "Subscribe failed with error %s\n", aws_error_debug_str(errorCode));
                        exit(-1);
                    }
                    else {
                        if (!packetId || QoS == AWS_MQTT_QOS_FAILURE) {
                            fprintf(stderr, "Subscribe rejected by the broker.");
                            exit(-1);
                        }
                        else {
                            fprintf(stdout, "Subscribe on topic %s on packetId %d Succeeded\n", topic.c_str(), packetId);
                        }
                    }
                    subscribeFinishedPromise.set_value();
                };

        connection->Subscribe(topic.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE, onMessage, onSubAck);
        subscribeFinishedPromise.get_future().wait();

//        uint32_t publishedCount = 0;
//        while (publishedCount < 3)
//        {
//            ByteBuf payload = ByteBufFromArray((const uint8_t *)"jhgjhgjhg", 9);
//
//            auto onPublishComplete = [](Mqtt::MqttConnection &, uint16_t, int) {};
//            connection->Publish(topic.c_str(), AWS_MQTT_QOS_AT_LEAST_ONCE, false, payload, onPublishComplete);
//            ++publishedCount;
//
//            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//        }

//        {
//            std::unique_lock<std::mutex> receivedLock(receiveMutex);
//            receiveSignal.wait(receivedLock, [&] { return true; });
//        }

//        while(true) {
//            std::this_thread::sleep_for(std::chrono::seconds(1));
//        }

//        // Unsubscribe from the topic.
//        std::promise<void> unsubscribeFinishedPromise;
//        connection->Unsubscribe(
//                topic.c_str(), [&](Mqtt::MqttConnection &, uint16_t, int) { unsubscribeFinishedPromise.set_value(); });
//        unsubscribeFinishedPromise.get_future().wait();

//        if (connection->Disconnect()) {
//            connectionClosedPromise.get_future().wait();
//        }
    }
    else {
        fprintf(stderr, "Not connected");
    }
}
