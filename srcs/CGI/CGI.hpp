#pragma once

#include <iostream>
#include <unistd.h>
#include "../KQueue/KQueue.hpp"

#ifndef M_DEBUG
# define M_DEBUG 1
#endif

#ifndef BUFF_SIZE
# define BUFF_SIZE 64000
#endif

#define NOCHILD -2

typedef enum request_method {
    POST,
    GET,
    DELETE
} t_method;

class CGI
{

public:
    static int runScript(t_method reqMethod, const char* cgiPath, const char* argv[], std::string& postData, long fd);
    static void readOutput(int fd, std::string& buff);

    static t_eventData m_cgiEventData;
    static void setupCGIEnvironment(HttpRequest* req) 
    {
        // Set required CGI environment variables
        setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
        setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
        setenv("REQUEST_METHOD", req->method.c_str(), 1);
        setenv("SCRIPT_NAME", req->uri.c_str(), 1);
        setenv("SCRIPT_FILENAME", req->uri.c_str(), 1);
        
        // Set query string if present
        size_t queryPos = req->uri.find('?');
        if (queryPos != std::string::npos) {
            setenv("QUERY_STRING", req->uri.substr(queryPos + 1).c_str(), 1);
        } else {
            setenv("QUERY_STRING", "", 1);
        }

        // Set content information for POST requests
        if (req->method == "POST") {
            auto contentLength = std::to_string(req->contentLength);
            setenv("CONTENT_LENGTH", contentLength.c_str(), 1);
            
            auto it = req->headers.find("Content-Type");
            if (it != req->headers.end()) {
                setenv("CONTENT_TYPE", it->second.c_str(), 1);
            }
        }
    }

    static int responseCGI(HttpRequest* req) 
    {
        int pipe_in[2];  // For writing to CGI process
        int pipe_out[2]; // For reading from CGI process
        
        if (pipe(pipe_in) < 0 || pipe(pipe_out) < 0) {
            throw std::runtime_error("Failed to create pipes");
        }

        pid_t pid = fork();
        if (pid < 0) {
            close(pipe_in[0]); close(pipe_in[1]);
            close(pipe_out[0]); close(pipe_out[1]);
            throw std::runtime_error("Fork failed");
        }

        if (pid == 0) { // Child process
            try {
                // Close unused pipe ends
                close(pipe_in[1]);  // Close write end of input pipe
                close(pipe_out[0]); // Close read end of output pipe

                // Redirect stdin to pipe_in
                if (dup2(pipe_in[0], STDIN_FILENO) < 0) {
                    throw std::runtime_error("Failed to redirect stdin");
                }

                // Redirect stdout to pipe_out
                if (dup2(pipe_out[1], STDOUT_FILENO) < 0) {
                    throw std::runtime_error("Failed to redirect stdout");
                }

                // Close the original file descriptors
                close(pipe_in[0]);
                close(pipe_out[1]);

                // Set up CGI environment variables
                setupCGIEnvironment(req);

                // Determine which interpreter to use
                std::string interpreter;
                std::string scriptPath = req->uri;
                if (scriptPath.ends_with(".php")) {
                    interpreter = "/usr/bin/php-cgi";  // Adjust path as needed
                } else if (scriptPath.ends_with(".py")) {
                    interpreter = "/usr/bin/python3";  // Adjust path as needed
                } else {
                    throw std::runtime_error("Unsupported CGI type");
                }

                // Execute the script
                char* const args[] = {
                    const_cast<char*>(interpreter.c_str()),
                    const_cast<char*>(scriptPath.c_str()),
                    nullptr
                };

                execv(args[0], args);
                // If execv returns, it failed
                throw std::runtime_error("Failed to execute CGI script");
            }
            catch (const std::exception& e) {
                std::cerr << "CGI error: " << e.what() << std::endl;
                exit(1);
            }
        }

        // Parent process
        // Close unused pipe ends
        close(pipe_in[0]);  // Close read end of input pipe
        close(pipe_out[1]); // Close write end of output pipe

        // Write request body to CGI process if POST method
        if (req->method == "POST" && !req->bodyFile.empty()) {
            int fd = open(req->bodyFile.c_str(), O_RDONLY);
            dup2(fd, pipe_in[1]);
        }

        // Close write pipe as we're done sending data
        close(pipe_in[1]);

        // Return the read end of the output pipe
        return pipe_out[0];
    }
};
