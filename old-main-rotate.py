import random
import threading
from PIL import ImageTk  # Pillow package
from PIL import Image as PILImage
from tkinter import *

WIDTH = 600
HEIGHT = 600

win = Tk()
win.title("Truck Canvas")
win.geometry("%sx%s" % (WIDTH, HEIGHT))
canvas = Canvas(win, width=WIDTH, height=HEIGHT, background="white")

# image = PhotoImage(file="truck01.png")
# resized_image = image.subsample(4, 4)  # larger number is smaller
# canvas.create_image(100, 100, anchor=CENTER, image=resized_image, tags="truck01")

image = PILImage.open('truck01.png')
photo = ImageTk.PhotoImage(image)
# resized_image = image.subsample(4, 4)  # larger number is smaller
canvas.create_image(image.width/2, image.height/2, image=photo)

rotated_image = image.rotate(45)
rotated_photo = ImageTk.PhotoImage(rotated_image)
canvas.itemconfigure(1, image=rotated_photo)


def move_truck(truck, x, y):
    canvas.coords(truck, x, y)


move_truck("truck01", 100, 200)

canvas.pack()

win.mainloop()
