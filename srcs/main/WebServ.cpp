#include "WebServ.hpp"

WebServ::WebServ()
    : m_cEventData("client socket", NULL)
    , m_openedSockets (0)
{
}

// returns -1 if kevent failed
int WebServ::handleNewConnection(struct kevent* current)
{
    t_eventData *serverEvData = (t_eventData*)current->udata;
    t_sockData* tmp = (t_sockData*)serverEvData->data;

	int clientSock = accept(current->ident, (sockaddr*)(tmp->sockAddress), tmp->sockLen);
    if (clientSock == -1)
        perror("accept(2)");

    KQueue::setFdNonBlock(clientSock);

    HttpRequest *req = new HttpRequest();
    t_eventData *clientEvData = new t_eventData("client socket", req);

	KQueue::watchFd(clientSock, clientEvData);
	return 0;
}

int WebServ::handleExistedConnection(struct kevent* current)
{
    // Read and Parse Request
    HttpRequest* req = (HttpRequest*) ((t_eventData*)current->udata)->data;

    req->readRequest(current->ident);

    if(req->isDone == true) {
        M_DEBUG && std::cerr << "-->" << req->total_read_bytes << "--" << req->content_length << '\n';

        std::string response = "HTTP/1.1 201 Created\r\n";
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: 0\r\n";
        response += "\r\n";
        send(current->ident, response.c_str(), response.size(), 0);

        delete req;
        delete (t_eventData*)current->udata;
        KQueue::removeFd(current->ident);
        close(current->ident);
        m_openedSockets--;

        std::cerr << "Closed Connection.\n";
        return 1;
    }
	return 0;
}

void WebServ::sendResponse(struct kevent* current)
{
    std::string content;

    CGI::readOutput(current->ident, content);


	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(content.length()) + "\n\n" + content;

    if (send((long)current->udata, response.c_str(), response.size(), 0) == -1) {
		(M_DEBUG) && std::cerr << "error while sending response: " << strerror(errno) << '\n' ;
	}

    close((long)current->udata);
}

void WebServ::run()
{
    KQueue::createKq();

    for (size_t i = 0; i < servers.size(); i++) {
        servers[i].init();

        EV_SET(&KQueue::m_keventBuff, servers[i].getSocket(), EVFILT_READ, EV_ADD, 0, 0, (void*)&(servers[i].m_sEventData));
        if (kevent(KQueue::getFd(), &KQueue::m_keventBuff, 1, 0, 0, 0) == -1) {
            if (M_DEBUG)
                perror("kevent(2)");
            close(servers[i].getSocket());
            throw std::runtime_error("There was an error while adding server socket to kqueue");
        }
        m_openedSockets++;
    }

    while (true)
    {
        struct kevent events[m_openedSockets];
        int nevents = KQueue::getEvents(events, m_openedSockets);

        for (int i = 0; i < nevents; i++) {

            if (!std::strcmp((static_cast<t_eventData*>(events[i].udata))->type, "server socket")) {

                handleNewConnection(&events[i]);
                m_openedSockets++;
				std::cout << "Connection accepted" << std::endl ;
            }
            else if (!std::strcmp(static_cast<t_eventData*>(events[i].udata)->type, "client socket")) {
				handleExistedConnection(&events[i]);
				// std::cout << "Request parsed" << std::endl ;
            }
            // else {
            //     sendResponse(&events[i]);
			// 	std::cout << "Response sent" << std::endl ;
            // }
        }
        // if (waitpid(-1, NULL, WNOHANG) == -1)
        //     std::perror("waitpid(2)");
    }
}
