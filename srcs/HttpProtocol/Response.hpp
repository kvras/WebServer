#pragma once

#include <iostream>
#include <vector>

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
    public :
        HttpResponse(std::string Uri, std::string Method, std::string BodyFile) : Version("HTTP/1.1"), ResponseCode("200 OK"), ContentType("text/html"), Connection("close"){
            this->Uri = Uri;
            this->Method = Method;
            this->BodyFile = BodyFile;
        }
        void SetVersion(std::string value);
        void SetResponseCode(std::string value);
        void SetContentType(std::string value);
        void SetConnection(std::string value);
        void SetBody(std::vector<char> Body);

        std::string GetVersion();
        std::string GetResponseCode();
        std::string GetContentType();
        std::string GetConnection();
        const std::vector<char>& GetBody();
        const std::vector<char> BuildResponse();

};

std::string WhatContentType(std::string uri);

