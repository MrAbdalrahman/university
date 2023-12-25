from http.server import SimpleHTTPRequestHandler, HTTPServer
from socketserver import ThreadingMixIn
import os
import urllib.parse

class ThreadedHTTPServer(ThreadingMixIn, HTTPServer):
    """Handle requests in a separate thread."""

class MyHandler(SimpleHTTPRequestHandler):
    def do_GET(self):
        parsed_path = urllib.parse.urlparse(self.path)
        file_path = parsed_path.path.lstrip("/")

        if file_path == "index.html":
            self.log_request_details("HTML")
            self.send_html_response("main_en.html")
        elif file_path == "ar":
            self.log_request_details("HTML")
            self.send_html_response("main_ar.html")
        elif file_path.endswith(".html"):
            self.log_request_details("HTML")
            self.send_html_file(file_path)
        elif file_path.endswith(".css"):
            self.log_request_details("CSS")
            self.send_css_file(file_path)
        elif file_path.endswith(".png"):
            self.log_request_details("PNG")
            self.send_image(file_path, "image/png")
        elif file_path.endswith(".jpg"):
            self.log_request_details("JPG")
            self.send_image(file_path, "image/jpeg")
        elif file_path in ["cr", "so", "rt"]:
            self.log_request_details("Redirect")
            self.send_redirect(file_path)
        else:
            self.log_request_details("Unknown")
            self.send_error_response()

    def log_request_details(self, file_type):
        request_line = self.requestline
        method, path, protocol = request_line.split(" ")
        print(f"Received request: Method: {method}, Path: {path}, Protocol: {protocol}, File Type: {file_type}")

    def send_static_file(self, file_path, content_type):
        # Construct the absolute path to the static file
        abs_file_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "static", file_path))

        # Check if the file exists
        if not os.path.exists(abs_file_path):
            self.send_error_response()
            return

        # Serve static files directly
        self.send_response(200)
        self.send_header("Content-type", content_type)
        self.end_headers()

        with open(abs_file_path, "rb") as file:
            self.copyfile(file, self.wfile)

    def send_html_response(self, filename):

        self.send_static_file(filename, "text/html")

    def send_html_file(self, file_path):
        self.send_static_file(file_path, "text/html")

    def send_css_file(self, file_path):
        self.send_static_file(file_path, "text/css")

    def send_image(self, file_path, content_type):
        self.send_static_file(file_path, content_type)

    def send_redirect(self, target):
        locations = {"cr": "http://cornell.edu", "so": "http://stackoverflow.com", "rt": "http://ritaj.birzeit.edu"}
        if target in locations:
            self.send_response(307)
            self.send_header("Location", locations[target])
            self.end_headers()
        else:
            self.send_error_response()

    def send_error_response(self):
        self.send_response(404)
        self.send_header("Content-type", "text/html")
        self.end_headers()

        # Open and send the 'DNE.html' file
        try:
            with open(os.path.join("static", "DNE.html"), "rb") as file:
                self.copyfile(file, self.wfile)
        except FileNotFoundError:
            # If 'DNE.html' is not found, send a simple error message
            self.wfile.write(b"<html><body><h1>File Not Found</h1></body></html>")

    def log_message(self, format, *args):
        # Print the HTTP requests to the terminal window
        print("%s - - [%s] %s\n" % (self.client_address[0], self.log_date_time_string(), format % args))

if __name__ == "__main__":
    port = 9966
    server_address = ('0.0.0.0', port)
    httpd = ThreadedHTTPServer(server_address, MyHandler)

    print(f"Server listening on port {port}")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("Server stopped by user")
