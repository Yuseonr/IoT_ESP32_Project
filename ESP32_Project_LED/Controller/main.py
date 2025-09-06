# LED Controller Drive
# MQTT using : mqtt
# GUI using : Tkinker

import tkinter as tk
import paho.mqtt.client as mqtt
import threading
import time

broker = "broker.hivemq.com"
topic = "IoT_Undip_ESP32"

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
status = False
pattern = '-'

def send_off():
    global status
    status = False
    print('send_off')
    client.publish(topic=topic, payload="0")
    update_status()

def send_on():
    global status
    status = True
    print('send_on')
    client.publish(topic=topic,payload="1")
    update_status()

def send_rainbow():
    global pattern
    pattern = 'RAINBOW'
    print('send_rainbow')
    client.publish(topic=topic, payload="2")
    update_status()

def send_snake():
    global pattern
    pattern = 'SNAKE'
    print('send_snake')
    client.publish(topic=topic, payload="3")
    update_status()

def send_fast():
    global pattern
    pattern = 'FAST'
    print('send_fast')
    client.publish(topic=topic, payload="4")
    update_status()

def send_arrow():
    global pattern
    pattern = 'ARROW'
    print('send_arrow')
    client.publish(topic=topic, payload="5")
    update_status()

def connect_mqtt():
    try:
        print("Attempting to connect...")
        client.connect(host=broker, port=1883, keepalive=60)
        print("Conected to HiveM")
        def mqtt_loop():
            client.loop_forever()
        mqtt_thread = threading.Thread(target=mqtt_loop, daemon=True)
        mqtt_thread.start()
    except Exception as e:
        print(e)

connect_mqtt()
time.sleep(2)

# ----------GUI----------

root = tk.Tk()
root.title("IoT LED Controller")

frame = tk.Frame(root, padx=100, pady=100)
frame.pack()

on_btn = tk.Button(frame, text="Turn on LED", command=send_on, fg='black')
on_btn.grid(row=0,column=0,padx=10)

off_btn = tk.Button(frame, text="Turn off LED", command=send_off, fg='black')
off_btn.grid(row=0,column=1,padx=10)

on_btn = tk.Button(frame, text="-Rainbow-", command=send_rainbow, fg='black')
on_btn.grid(row=1,column=0,padx=10, pady=10)

off_btn = tk.Button(frame, text="--Snake--", command=send_snake, fg='black')
off_btn.grid(row=1,column=1,padx=10)

on_btn = tk.Button(frame, text="--Fast--", command=send_fast, fg='black')
on_btn.grid(row=2,column=0,padx=10)

off_btn = tk.Button(frame, text="--Arrow--", command=send_arrow, fg='black')
off_btn.grid(row=2,column=1,padx=10)

status_label = tk.Label(frame, text= f"Status : {"ON" if status else "OFF"}")
status_label.grid(row=4,column=0,columnspan=2,pady=10)

pattern_label = tk.Label(frame, text= f"Pattern : {pattern}")
pattern_label.grid(row=5,column=0,columnspan=2,pady=10)

def update_status():
    status_label.config(text=f"Status : {'ON' if status else 'OFF'}")
    pattern_label.config(text=f"Pattern : {pattern}")

root.mainloop()