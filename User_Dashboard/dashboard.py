import paho.mqtt.client as mqtt
from datetime import datetime
import tkinter as tk
import threading
import sys

BROKER_IP = "YOUR_BROKER_IP"
BROKER_PORT = 1883
TOPIC = "fall/detection"

status_label = None
log_label = None
root = None

def on_connect(client, userdata, flags, rc):
    print(f"Connected to broker (rc={rc})")
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    alert = msg.payload.decode()
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    print(f">> {alert} at {timestamp}")

    with open("fall_log.csv", "a") as f:
        f.write(f"{timestamp},{alert}\n")

    if root:
        root.after(0, update_gui, alert, timestamp)

def update_gui(alert, timestamp):
    if "FALL" in alert:
        status_label.config(text="FALL", fg="red")
    else:
        status_label.config(text="OK", fg="green")
    log_label.config(text=f"Last: {alert} @ {timestamp}")


def start_mqtt():
    try:
        client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1)
        client.on_connect = on_connect
        client.on_message = on_message
        client.connect(BROKER_IP, BROKER_PORT, 60)
        client.loop_forever()
    except Exception as e:
        print(f"MQTT connection error: {e}")
        sys.exit(1)


def main():
    global root, status_label, log_label

    root = tk.Tk()
    root.title("Fall Detection Dashboard")
    root.geometry("400x250")
    root.configure(bg="white")

    tk.Label(root, text="Fall Detection", font=("Arial", 16, "bold"),
             bg="white").pack(pady=10)

    tk.Label(root, text="Status:", font=("Arial", 12),
             bg="white").pack()

    status_label = tk.Label(root, text="OK", font=("Arial", 48, "bold"),
                            fg="green", bg="white")
    status_label.pack(pady=10)

    log_label = tk.Label(root, text="Waiting for data...",
                         font=("Arial", 10), bg="white", fg="gray")
    log_label.pack(pady=10)

    mqtt_thread = threading.Thread(target=start_mqtt, daemon=True)
    mqtt_thread.start()

    root.mainloop()


if __name__ == "__main__":
    main()