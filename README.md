![webServer](webServer.gif "webServer")

# Build Your Own Web Server
creating a high-performance, secure web server from scratch using C++. This server is capable of handling thousands of concurrent requests while supporting essential web functionalities such as HTTP parsing, dynamic content generation, and CGI execution.
Along the way, I learned about socket programming, TCP/IP, and implementing secure coding practices and more ...

# Features
HTTP Protocol Support: Handles HTTP methods like GET, POST, and DELETE.
Concurrent Request Handling: Efficiently manages up to 1,000 simultaneous connections using multithreading.
Dynamic Content with CGI: Executes external programs or scripts for dynamic content generation.
Static File Delivery: Serves files like HTML, CSS, and JavaScript for building static websites.
Custom HTTP Parser: Parses HTTP requests and generates appropriate responses while adhering to standards.
Environment Variables: Supports $VAR_NAME for dynamic configuration during runtime.
Signal Management: Gracefully handles SIGINT, SIGTERM, and other termination signals for clean shutdowns.
Logging: Logs requests, responses, and errors for debugging and monitoring.

## Building and Running

To compile the project, run:

```bash
make

To run the project, use:

./webserver ip:port