#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <algorithm>
#include "Exceptions.hpp"

enum ParseState{
    FirstLine,
    Headers,
    Body
};

class HttpRequest
{

public:
    HttpRequest();
    void readRequest(int fd);
    void performChecks(void);
    void parseRequest(char *request, size_t size);
    void parseFirstLine(std::string line);
    void parseHeaders(std::string line);
    void parseBody(char *line, size_t size);
    void generateUniqueFile(void);
    void unchunkBody(char *request, size_t size);
    bool isDone;
    long content_length;
    long total_read_bytes;
    std::string method ,uri, version, boundary, bodyFile;
    std::string getHeader(std::string key);
private:
    double chunk_size, bodyRead;
    std::map<std::string, std::string> headers;
    std::vector<char> partial_data;
    ParseState state;
    ssize_t read_bytes;
    long long chunkPos;
    std::string TransferEncoding;
    bool skipNextLine;

};


#endif
