#include "../headers/client.h"
#include "../headers/client_functions.h"

using namespace std::chrono;


void TestWithClients()
{   
    
    MyClient::TcpClient o_client;              
    o_client.setPort(3000);             
    o_client.createSocket();
    o_client.connectToServer();
    o_client.send_msg("");
    o_client.receive();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    o_client.send_msg("exit");
}

void startClient(int argc, char **argv)
{
    MyClient::TcpClient o_client;                // new client
                                      
        std::string message;                        
        std::cout<<"Enter port : ";              
        int tempPort = 0;
        std::cin>>tempPort;
        o_client.setPort(tempPort);             
        o_client.createSocket();
        o_client.connectToServer();
        while (o_client.send_msg() != "exit")
        {
            o_client.receive();
        }
          
    
}
