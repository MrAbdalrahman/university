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
            self.send_html_response("main_en.html")
        elif file_path == "ar":
            self.send_html_response("main_ar.html")
        elif file_path in ["cr", "so", "rt"]:
            self.send_redirect(file_path)
        else:
            # Treat any other request as a request for a static file
            self.send_static_file(file_path)

    def send_static_file(self, file_path):
        # Construct the absolute path to the static file
        abs_file_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "static", file_path))

        # Check if the file exists
        if not os.path.exists(abs_file_path):
            self.send_error(404, "File Not Found")
            return

        # Serve static files directly
        self.send_response(200)
        self.send_header("Content-type", self.guess_type(abs_file_path))
        self.end_headers()

        with open(abs_file_path, "rb") as file:
            self.copyfile(file, self.wfile)

    def send_html_response(self, filename):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

        with open(os.path.join("static", filename), "rb") as file:
            self.copyfile(file, self.wfile)

    def send_redirect(self, target):
        locations = {"cr": "http://cornell.edu", "so": "http://stackoverflow.com", "rt": "http://ritajwebsite.com"}
        if target in locations:
            self.send_response(307)
            self.send_header("Location", locations[target])
            self.end_headers()
        else:
            self.send_error(404, "File Not Found")

if __name__ == "__main__":
    port = 9966
    server_address = ('0.0.0.0', port)
    httpd = ThreadedHTTPServer(server_address, MyHandler)

    print(f"Server listening on port {port}")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("Server stopped by user")
