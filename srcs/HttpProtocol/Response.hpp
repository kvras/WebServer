#pragma once

#include <iostream>
#include <vector>
#include "Request.hpp"
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "../CGI/CGI.hpp"
class HttpRequest;
class HttpResponse{
    private :
        std::string Version;
        std::string ResponseCode;
        std::string ContentType;
        std::string Connection;
        std::vector<char> Body;
        std::string Uri;
        std::string Method;
        std::string BodyFile;
        int fd;
        int clientFd;
    public :
        bool ResponseDelivred;
        HttpResponse(int fd, int clientFd, std::string uri, std::string method, std::string bodyFile){
            this->Uri = uri;
            this->Method = method;
            this->BodyFile = bodyFile;
            this->ResponseDelivred = false;
        }
        void SetVersion(std::string value);
        void SetResponseCode(std::string value);
        void SetContentType(std::string value);
        void SetConnection(std::string value);
        void SetBody(std::vector<char> Body);
        // void DeliveringResponse(std::string& uri, std::string& method, std::string& bodyFile);

        std::string GetVersion();
        std::string GetResponseCode();
        std::string GetContentType();
        std::string GetConnection();
        const std::vector<char>& GetBody();
        const std::vector<char> BuildResponse();
        static int responseCGI(HttpRequest *req);
        static int responseFile(HttpRequest &req);

};

std::string WhatContentType(std::string uri);

