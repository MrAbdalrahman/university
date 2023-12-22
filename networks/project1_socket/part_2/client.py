import socket
import threading

PORT = 9955  # using port 9955
FORMAT = "utf-8"  # format used for our messages decoding
D = "DES"
HEADER = 64
SERVER = "26.22.62.239"  # my ip address
ADDR = (SERVER, PORT)  # adding the server and the port
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # using ipv4 address family and type stream for the client
client.connect(ADDR)


def send(msg):
    MSG = msg.encode(FORMAT)  # encoding the decoded message with the same format
    msg_len = len(MSG)  # message length got
    send_len = str(msg_len).encode(FORMAT)
    send_len +=b' '*(HEADER - len(send_len))
    client.send(send_len)
    client.send(MSG)


def read():
    msg_len = client.recv(HEADER).decode(FORMAT)  # take the message header and check the length and allocate 64
    if msg_len:
        msg_len = int(msg_len)  # now we have the message length
        msg = client.recv(msg_len).decode(FORMAT)
        print(f"{msg} ")


x = ''
while 1:
    x = input()
    if x!='DES':
         send(x)
    thread = threading.Thread(target=read)
    thread.start()
send(D)