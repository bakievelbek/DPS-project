import command_line_utils
import json
import sys
from awscrt import mqtt
from tkinter import *

WIDTH = 1200
HEIGHT = 700
TEXT_CONTAINER_WIDTH = 340
TEXT_CONTAINER_HEIGHT = HEIGHT / 5

truck_colours = [
    'black',
    'red',
    'blue',
    'green',
    'magenta',
    'yellow'
]

model = {}
truck_color = {}

win = Tk()
win.title("Truck Canvas")
win.geometry("%sx%s" % (WIDTH, HEIGHT))
canvas = Canvas(win, width=WIDTH, height=HEIGHT, background="white")

image = PhotoImage(file="images/truck01.png")
resized_image = image.subsample(4, 4)  # larger number is smaller
canvas.create_image(100, 100, anchor=CENTER, image=resized_image, tags="truck01")

canvas.coords("truck01", -200, -200)

image2 = PhotoImage(file="images/truck02.png")
resized_image2 = image2.subsample(4, 4)  # larger number is smaller
canvas.create_image(100, 100, anchor=CENTER, image=resized_image2, tags="truck02")
canvas.coords("truck02", -200, -200)

image3 = PhotoImage(file="images/truck03.png")
resized_image3 = image3.subsample(4, 4)  # larger number is smaller
canvas.create_image(100, 100, anchor=CENTER, image=resized_image3, tags="truck03")
canvas.coords("truck03", -200, -200)

image4 = PhotoImage(file="images/truck04.png")
resized_image4 = image4.subsample(4, 4)  # larger number is smaller
canvas.create_image(100, 100, anchor=CENTER, image=resized_image4, tags="truck04")
canvas.coords("truck04", -200, -200)

image5 = PhotoImage(file="images/truck05.png")
resized_image5 = image5.subsample(4, 4)  # larger number is smaller
canvas.create_image(100, 100, anchor=CENTER, image=resized_image5, tags="truck05")
canvas.coords("truck05", -200, -200)

image6 = PhotoImage(file="images/truck06.png")
resized_image6 = image6.subsample(4, 4)  # larger number is smaller
canvas.create_image(100, 100, anchor=CENTER, image=resized_image6, tags="truck06")
canvas.coords("truck06", -200, -200)

canvas.pack()

received_count = 0
cmdUtils = command_line_utils.CommandLineUtils("PubSub - Send and recieve messages through an MQTT connection.")
cmdUtils.add_common_mqtt_commands()
cmdUtils.add_common_topic_message_commands()
cmdUtils.add_common_proxy_commands()
cmdUtils.add_common_logging_commands()
cmdUtils.register_command("endpoint", "<str>", "endpoint", True, str)
cmdUtils.register_command("key", "<path>", "Path to your key in PEM format.", True, str)
cmdUtils.register_command("cert", "<path>", "Path to your client certificate in PEM format.", True, str)
cmdUtils.register_command("client_id", "<str>", "Client ID to use for MQTT connection (optional, default='test-*').")
cmdUtils.register_command("topic", "platoon/channel", "topic", True, str)
cmdUtils.register_command("port", "<int>", "Connection port. AWS IoT supports 443 and 8883 (optional, default=auto).",
                          type=int)

cmdUtils.get_args()


# Callback when connection is accidentally lost.
def on_connection_interrupted(connection, error, **kwargs):
    print("Connection interrupted. error: {}".format(error))


def on_connection_resumed(connection, return_code, session_present, **kwargs):
    print("Connection resumed. return_code: {} session_present: {}".format(return_code, session_present))

    if return_code == mqtt.ConnectReturnCode.ACCEPTED and not session_present:
        print("Session did not persist. Resubscribing to existing topics...")
        resubscribe_future, _ = connection.resubscribe_existing_topics()

        resubscribe_future.add_done_callback(on_resubscribe_complete)


def on_resubscribe_complete(resubscribe_future):
    resubscribe_results = resubscribe_future.result()
    print("Resubscribe results: {}".format(resubscribe_results))

    for topic, qos in resubscribe_results['topics']:
        if qos is None:
            sys.exit("Server rejected resubscribe to topic: {}".format(topic))


# Callback when the subscribed topic receives a message
def on_message_received(topic, payload, dup, qos, retain, **kwargs):
    data = json.loads(payload.decode('utf-8'))
    model[data['id']] = data
    if data['id'] not in truck_color:
        truck_color[data['id']] = len(truck_color) % 6 + 1


def update_screen():
    # delete the old text elements
    for element in canvas.find_withtag("truck_text"):
        canvas.delete(element)

    for truck_id in dict(model):  # make a copy to prevent mutex issues
        truck = model[truck_id]
        colour_id = truck_color[truck_id]

        # move truck
        canvas.coords(f"truck0{colour_id}", truck["x"], truck["y"])

        text = f"""
            Truck ID: {truck['id']}
                x: {truck["x"]}
                y: {truck["y"]}
                is-braking: {truck["is-braking"]}
                speed: {truck['speed']}
                direction: {truck['direction']}
                joined: {truck["joined"] != 0}
        """

        canvas.create_text(
            WIDTH - TEXT_CONTAINER_WIDTH,
            (colour_id - 1) * TEXT_CONTAINER_HEIGHT,
            text=text,
            fill='white',
            tags='truck_text',
            anchor='nw'
        )

    canvas.after(100, update_screen)


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

    for i in range(6):
        canvas.create_rectangle(
            WIDTH - TEXT_CONTAINER_WIDTH,
            i * TEXT_CONTAINER_HEIGHT,
            WIDTH,
            (i + 1) * TEXT_CONTAINER_HEIGHT,
            outline='black',
            fill=truck_colours[i],
            width=2
        )

    canvas.after(1000, update_screen)

    win.mainloop()
