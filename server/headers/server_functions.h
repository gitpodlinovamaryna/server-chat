#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include "server.h"
extern std::vector<int> socketsVector;

std::string getHostStr(const TcpServer::Client& client);
void startServer();


#endif