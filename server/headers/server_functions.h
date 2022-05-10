#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include "server.h"

std::string getHostStr(const TcpServer::Client& client);
int startServer();


#endif