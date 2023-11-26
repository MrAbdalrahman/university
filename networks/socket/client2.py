import socket
import threading
from queue import Queue

PORT = 5050
FORMAT = "utf-8"
D = "DES"
HEADER = 64
SERVER = "26.22.62.239"
ADDR = (SERVER, PORT)
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(ADDR)

# Add locks and queue for thread synchronization
client_lock = threading.Lock()
print_lock = threading.Lock()
message_queue = Queue()


def send(msg):
    MSG = msg.encode(FORMAT)
    msg_len = len(MSG)
    send_len = str(msg_len).encode(FORMAT)
    send_len += b' ' * (HEADER - len(send_len))

    with client_lock:
        client.send(send_len)
        client.send(MSG)


def read():
    while True:
        with client_lock:
            msg_len = client.recv(HEADER).decode(FORMAT)

        if msg_len:
            msg_len = int(msg_len)

            with client_lock:
                msg = client.recv(msg_len).decode(FORMAT)

            with print_lock:
                print(f"{msg} ")


# Start the reading thread
read_thread = threading.Thread(target=read)
read_thread.start()

x = ''
while True:
    x = input()
    if x != 'DES':
        send(x)

# The following code block will never be reached because the input loop is infinite
# Keeping it here for reference, you might want to include some condition to break out of the loop

# send(D)
# read_thread.join()  # Wait for the reading thread to finish before exiting
