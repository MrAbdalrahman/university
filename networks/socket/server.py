import socket
import threading

PORT = 5050  # using port 5050
FORMAT = "utf-8"  # format used for our messages decoding
HEADER = 64
D = "DES"
SERVER = socket.gethostbyname(socket.gethostname())  # dynamic ip address getter
ADDR = (SERVER, PORT)  # adding the server and the port
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # using ipv4 address family and type stream for the server
server.bind(ADDR)  # binding the server and the port
# sever is built and ready to use


def send(msg, client):
    MSG = msg.encode(FORMAT)  # encoding the decoded message with the same format
    msg_len = len(MSG)  # message length got
    send_len = str(msg_len).encode(FORMAT)
    send_len +=b' '*(HEADER - len(send_len))
    client.send(send_len)
    client.send(MSG)


def client(conn, addr, clients):

    print(f"{addr} Is Connected!")
    connected = True  # connection status
    while connected:
        msg_len = conn.recv(HEADER).decode(FORMAT)  # take the message header and check the length and allocate 64
        if msg_len:
            msg_len = int(msg_len)  # now we have the message length
            msg = conn.recv(msg_len).decode(FORMAT)
            if msg == D:
                connected = False
                print(f"{addr} disconnected")
            print(f"{addr} {msg} ")
            for client in clients:   # sending message to all connections except the sender
                if client != conn:
                    send(msg, client)
    conn.close()


def start():
    server.listen()  # setting the server to listening mode
    print(f"The Server Is Listening to {SERVER} ")
    clients = []
    while True:  # this will receive any connection
        conn, addr = server.accept()  # obtaining connection and ip address
        clients.append(conn)  # appending connections
        thread = threading.Thread(target=client, args=(conn, addr, clients))
        thread.start()


print("The Server Is Starting")
start()
