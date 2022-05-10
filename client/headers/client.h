#ifndef CLIENT_H
#define CLIENT_H


#include <iostream> 
#include <cstdio> 
#include <cstring> 
#include <string> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netdb.h> 
#include <netinet/in.h>	   
#include <netinet/tcp.h> 
#include <unistd.h>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>


namespace MyClient
{
    class TcpClient
    {
        private:
            int m_socket;
            int m_port;
            int m_keepalive;
            char m_msg[1024];
            std::string m_ipAddress;
            struct sockaddr_in serv_addr;
            


        public:
            TcpClient();                       // Initializer
            TcpClient(int);                    // Initializer with param m_port
            ~TcpClient();                      // Destructor
            void setPort(int);                  
            void createSocket();                    
            void connectToServer();
            std::string send_msg();
            void send_msg(std::string);
            void receive();
    };

}

#endif
