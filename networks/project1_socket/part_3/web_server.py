import socket
import os
import urllib.parse
from threading import Thread


class MyHTTPServer:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)

    def start(self):
        print(f"Server listening on {self.host}:{self.port}")
        while True:
            client_socket, client_address = self.server_socket.accept()
            client_handler = Thread(target=self.handle_client, args=(client_socket,))
            client_handler.start()



    def handle_client(self, client_socket):
        client_address = client_socket.getpeername()
        request_data = client_socket.recv(1024).decode("utf-8")
        if not request_data:
            return

        method, path, _ = request_data.split(" ", 2)
        path = urllib.parse.unquote(path)
        file_path = path.lstrip("/")

        if method == "GET":
            if file_path == "index.html" or file_path == "en" or file_path == "":
                status_code = "200"
                self.log_request_details(client_address, method, path, "HTML", status_code)
                self.send_html_response(client_socket, "main_en.html")
            elif file_path == "ar":
                status_code = "200"
                self.log_request_details(client_address, method, path, "HTML", status_code)
                self.send_html_response(client_socket, "main_ar.html")
            elif file_path.endswith(".html"):
                status_code = "200"
                self.log_request_details(client_address, method, path, "HTML", status_code)
                self.send_html_file(client_socket, file_path)
            elif file_path.endswith(".css"):
                status_code = "200"
                self.log_request_details(client_address, method, path, "CSS", status_code)
                self.send_css_file(client_socket, file_path)
            elif file_path.endswith(".png"):
                status_code = "200"
                self.log_request_details(client_address, method, path, "PNG", status_code)
                self.send_image(client_socket, file_path, "image/png")
            elif file_path.endswith(".jpg"):
                status_code = "200"
                self.log_request_details(client_address, method, path, "JPG", status_code)
                self.send_image(client_socket, file_path, "image/jpeg")
            elif file_path in ["cr", "so", "rt"]:
                status_code = "307"
                self.log_request_details(client_address, method, path, "Redirect", status_code)
                self.send_redirect(client_socket, file_path)
            else:

                status_code = "404"
                self.log_request_details(client_address, method, path, "Unknown", status_code)
                self.send_error_response(client_socket)
        else:
            self.send_error_response(client_socket)

        client_socket.close()

    def log_request_details(self, client_address, method, path, file_type, status_code):
        print(f"Received request from {client_address}: Method: {method}, Path: {path}, File Type: {file_type}, Status Code: {status_code}")

    def send_static_file(self, client_socket, file_path, content_type):
        abs_file_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "static", file_path))
        if not os.path.exists(abs_file_path):
            self.send_error_response(client_socket)
            return

        with open(abs_file_path, "rb") as file:
            content = file.read()

        response = f"HTTP/1.1 200 OK\r\nContent-Type: {content_type}\r\nContent-Length: {len(content)}\r\n\r\n"
        client_socket.sendall(response.encode("utf-8") + content)

    def send_html_response(self, client_socket, filename):
        self.send_static_file(client_socket, filename, "text/html")

    def send_html_file(self, client_socket, file_path):
        self.send_static_file(client_socket, file_path, "text/html")

    def send_css_file(self, client_socket, file_path):
        self.send_static_file(client_socket, file_path, "text/css")

    def send_image(self, client_socket, file_path, content_type):
        self.send_static_file(client_socket, file_path, content_type)

    def send_redirect(self, client_socket, target):
        locations = {"cr": "http://cornell.edu", "so": "http://stackoverflow.com", "rt": "http://ritaj.birzeit.edu"}
        if target in locations:
            response = f"HTTP/1.1 307 Temporary Redirect\r\nLocation: {locations[target]}\r\n\r\n"
            client_socket.sendall(response.encode("utf-8"))
        else:
            self.send_error_response(client_socket)

    def send_error_response(self, client_socket):

        response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"

        try:
            with open(os.path.join("static", "DNE.html"), "rb") as file:
                content = file.read()

            client_socket.sendall(response.encode("utf-8") + content)
        except FileNotFoundError:

            error_message = "<html><body><h1>File Not Found</h1></body></html>"
            client_socket.sendall(response.encode("utf-8") + error_message.encode("utf-8"))


if __name__ == "__main__":
    host = '0.0.0.0'
    port = 9966
    server = MyHTTPServer(host, port)

    try:
        server.start()
    except KeyboardInterrupt:
        print("Server stopped by user")
