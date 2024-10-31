#pragma once


#include <iostream>
#include <exception>
#include "Response.hpp"


#ifndef M_DEBUG
# define M_DEBUG 1
#endif


class ErrorStatus;
class SuccessStatus;


class ErrorStatus {
public:
    // You can Pass NUll to debugMsg
    ErrorStatus(int errorCode, const char* debugMsg)
        : errorCode (errorCode)
        , headers ("HTTP/1.1 " + std::to_string(errorCode) + "\r\nContent-Type: text/html\r\n")
    {
        codeString = getCodeString(errorCode);

        headers += codeString + "\r\nConnection: close\r\n\r\n";

        if (M_DEBUG && debugMsg)
            std::cerr << debugMsg << "\n";
        switch (errorCode)
        {
            case 400:
                statusMessage = headers + "<html><head><title>400 Bad Request</title></head><body><center><h1>400 Bad Request</h1></center><hr></body></html>";
                break;
        
            case 401:
                statusMessage = headers + "<html><head><title>401 Unauthorized</title></head><body><center><h1>401 Unauthorized</h1></center><hr></body></html>";
                break;

            case 403:
                statusMessage = headers + "<html><head><title>403 Forbidden</title></head><body><center><h1>403 Forbidden</h1></center><hr></body></html>";
                break;

            case 404:
                statusMessage = headers + "<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></center><hr></body></html>";
                break;

            case 500:
                statusMessage = headers + "<html><head><title>500 Internal Server Error</title></head><body><center><h1>500 Internal Server Error</h1></center><hr></body></html>";
                break;

            case 501:
                statusMessage = headers + "<html><head><title>501 Not Implemented</title></head><body><center><h1>501 Not Implemented</h1></center><hr></body></html>";
                break;

            case 502:
                statusMessage = headers + "<html><head><title>502 Bad Gateway</title></head><body><center><h1>502 Bad Gateway</h1></center><hr></body></html>";
                break;

            case 503:
                statusMessage = headers + "<html><head><title>503 Service Unavailable</title></head><body><center><h1>503 Service Unavailable</h1></center><hr></body></html>";
                break;

            case 504:
                statusMessage = headers + "<html><head><title>504 Gateway Timeout</title></head><body><center><h1>504 Gateway Timeout</h1></center><hr></body></html>";
                break;
            // Add more status codes
            // ..
            default:
                M_DEBUG && std::cerr << "Error: Unknown success code\n";
                statusMessage = headers + "<html><head><title>500 Internal Error</title></head><body><center><h1>500 Internal Error</h1></center><hr></body></html>";
                break;
        }
    }
    const char* what() const throw() {return (statusMessage.c_str());}
    int errorCode;
    std::string headers;
    std::string statusMessage;
    std::string codeString;
};

class SuccessStatus {
public:
    // You can Pass NUll to debugMsg
    SuccessStatus(int successCode, const char* debugMsg)
        : successCode (successCode)
        , headers ("HTTP/1.1 " + std::to_string(successCode) + "\r\nContent-Type: text/html\r\n")
    {
        codeString = getCodeString(successCode);

        headers += codeString + "\r\nConnection: close\r\n\r\n";

        if (M_DEBUG && debugMsg)
            std::cerr << debugMsg << "\n";
        switch (successCode)
        {
            case 200:
                statusMessage = headers + "<html><head><title>200 OK</title></head><body><center><h1>200 OK</h1></center><hr></body></html>";
                break;
            
            case 201:
                statusMessage = headers + "<html><head><title>201 Created</title></head><body><center><h1>201 Created</h1></center><hr></body></html>";
                break;

            case 202:
                statusMessage = headers + "<html><head><title>202 Accepted</title></head><body><center><h1>202 Accepted</h1></center><hr></body></html>";
                break;

            case 204:
                statusMessage = headers + "<html><head><title>204 No Content</title></head><body><center><h1>204 No Content</h1></center><hr></body></html>";
                break;
            
            // Add more status codes
            // ..
            default:
                M_DEBUG && std::cerr << "Error: Unknown success code\n";
                throw ErrorStatus(500, NULL);
                break;
        }
    }
    const char* what() const throw() {return (statusMessage.c_str());}
    int successCode;
    std::string headers;
    std::string statusMessage;
    std::string codeString;
};
