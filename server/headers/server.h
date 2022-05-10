#ifndef SERVER_H
#define SERVER_H

#include <cstdint>
#include <functional>
#include <thread>
#include <list>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> 
#include <netdb.h> 
#include <netinet/tcp.h> 
#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <cstring>

//Buffer for receiving data from the client
static constexpr uint16_t buffer_size = 4096;
extern std::vector<int> socketsVector;

class TcpServer 
{
public:
    class Client;
    
     //Client handler callback type
    typedef std::function<void(Client)> handler_function_t;
    
     //Server Status
    enum class status : uint8_t 
    {
        up = 0,
        err_socket_init = 1,
        err_socket_bind = 2,
        err_socket_listening = 3,
        close = 4
    };

private:

    uint16_t port; 
    status _status = status::close;
    handler_function_t handler;
    std::thread handler_thread;
    std::string m_ipAddress = "127.0.0.1";
    int m_keepalive = 0;
    std::map <std::thread::id, int> ClientsList;
    std::list<std::thread> client_handler_threads;
    std::list<std::thread::id> client_handling_end;
    
    int serv_socket;
    void handlingLoop();

public:

    TcpServer(const uint16_t port, handler_function_t handler);
    ~TcpServer();
    
    
    //! Set client handler
    void setHandler(handler_function_t handler);
    bool checkBlock();

    uint16_t getPort() const;
    uint16_t setPort(const uint16_t port);
    status getStatus() const {return _status;}

    status restart();
    status start();
    void stop();
    void joinLoop();

};

class TcpServer::Client 
{

    int socket;
    struct sockaddr_in address;
    char buffer[buffer_size];
    std::string clientName;

public:
    std::mutex  lockprint;
    Client(int socket, struct sockaddr_in address);
    Client(const Client& other);
    bool operator ==(const Client & other)const;
    ~Client();
    bool checkBlock(char *str);
    void setName(std::string name);
    uint32_t getHost() const;
    uint16_t getPort() const;
    void sendMsg();
    std::string receiveMsg();
    void messageExchange(std::string);
    
};

#endif 