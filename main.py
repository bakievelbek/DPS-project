import command_line_utils
import json
import sys
import threading
import time
from awscrt import mqtt
from tkinter import *


WIDTH = 650
HEIGHT = 650

win = Tk()
win.title("Truck Canvas")
win.geometry("%sx%s" % (WIDTH, HEIGHT))
canvas = Canvas(win, width=WIDTH, height=HEIGHT, background="white")

image = PhotoImage(file="images/truck01.png")
resized_image = image.subsample(4, 4)  # larger number is smaller
canvas.create_image(100, 100, anchor=CENTER, image=resized_image, tags="truck01")

canvas.pack()

received_count = 0
cmdUtils = command_line_utils.CommandLineUtils("PubSub - Send and recieve messages through an MQTT connection.")
cmdUtils.add_common_mqtt_commands()
cmdUtils.add_common_topic_message_commands()
cmdUtils.add_common_proxy_commands()
cmdUtils.add_common_logging_commands()
cmdUtils.register_command("endpoint", "<str>", "endpoint", True, str)
cmdUtils.register_command("key",
                          "certs/985f54b692625fa585d1e985ab82d7fb9b18e43ced9445768f045c503bf2fb17-private.pem.key",
                          "Path to your key in PEM format.", True, str,
                          default='certs/985f54b692625fa585d1e985ab82d7fb9b18e43ced9445768f045c503bf2fb17-private.pem.key')
cmdUtils.register_command("cert",
                          "certs/985f54b692625fa585d1e985ab82d7fb9b18e43ced9445768f045c503bf2fb17-certificate.pem.crt",
                          "Path to your client certificate in PEM format.", True, str,
                          default='certs/985f54b692625fa585d1e985ab82d7fb9b18e43ced9445768f045c503bf2fb17-certificate.pem.crt')
cmdUtils.register_command("client_id", "<str>", "Client ID to use for MQTT connection (optional, default='test-*').")
cmdUtils.register_command("topic", "platoon/channel", "topic", True, str)
cmdUtils.register_command("port", "<int>", "Connection port. AWS IoT supports 443 and 8883 (optional, default=auto).",
                          type=int)

cmdUtils.get_args()
received_all_event = threading.Event()


# Callback when connection is accidentally lost.
def on_connection_interrupted(connection, error, **kwargs):
    print("Connection interrupted. error: {}".format(error))


# Callback when an interrupted connection is re-established.
def on_connection_resumed(connection, return_code, session_present, **kwargs):
    print("Connection resumed. return_code: {} session_present: {}".format(return_code, session_present))

    if return_code == mqtt.ConnectReturnCode.ACCEPTED and not session_present:
        print("Session did not persist. Resubscribing to existing topics...")
        resubscribe_future, _ = connection.resubscribe_existing_topics()

        # Cannot synchronously wait for resubscribe result because we're on the connection's event-loop thread,
        # evaluate result with a callback instead.
        resubscribe_future.add_done_callback(on_resubscribe_complete)


def on_resubscribe_complete(resubscribe_future):
    resubscribe_results = resubscribe_future.result()
    print("Resubscribe results: {}".format(resubscribe_results))

    for topic, qos in resubscribe_results['topics']:
        if qos is None:
            sys.exit("Server rejected resubscribe to topic: {}".format(topic))


# Callback when the subscribed topic receives a message
def on_message_received(topic, payload, dup, qos, retain, **kwargs):
    # print("Received message from topic '{}': {}".format(topic, payload))
    data = json.loads(payload.decode('utf-8'))
    canvas.coords("truck01", data['x'], data['y'])


if __name__ == '__main__':
    mqtt_connection = cmdUtils.build_mqtt_connection(on_connection_interrupted, on_connection_resumed)
    connect_future = mqtt_connection.connect()

    connect_future.result()
    print("Connected!")

    message_topic = cmdUtils.get_command(cmdUtils.m_cmd_topic)
    print("Subscribing to topic '{}'...".format(message_topic))
    subscribe_future, packet_id = mqtt_connection.subscribe(
        topic=message_topic,
        qos=mqtt.QoS.AT_LEAST_ONCE,
        callback=on_message_received)
    subscribe_result = subscribe_future.result()
    print("Subscribed with {}".format(str(subscribe_result['qos'])))

    win.mainloop()

    # while True:
    #     time.sleep(1)

    # received_all_event.wait()
    # print("{} message(s) received.".format(received_count))

    # print("Disconnecting...")
    # disconnect_future = mqtt_connection.disconnect()
    # disconnect_future.result()
    # print("Disconnected!")
