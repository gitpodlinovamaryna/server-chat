#include "../headers/server_functions.h"


//Parser ip to std::string
std::string getHostStr(const TcpServer::Client& client) 
{
  uint32_t ip = client.getHost ();
  return std::string() + std::to_string(int(reinterpret_cast<char*>(&ip)[0])) + '.' +
         std::to_string(int(reinterpret_cast<char*>(&ip)[1])) + '.' +
         std::to_string(int(reinterpret_cast<char*>(&ip)[2])) + '.' +
         std::to_string(int(reinterpret_cast<char*>(&ip)[3])) + ':' +
         std::to_string( client.getPort ());
}


int startServer()
{

  //Creating a TcpServer object with port and lambda function arguments for client processing
  TcpServer server( 3000,
      [&](TcpServer::Client client){
        //Outputting the address of the connected client to the console
        //Set client name
          client.setName(getHostStr(client));
        //Start receive and send message
          client.messageExchange((getHostStr(client)));
      }
  );

  //Server start
  if(server.start() == TcpServer::status::up) 
  {
    //If the server is running, display a message and enter the client wait thread
      std::cout<<"Server is up!"<<std::endl;
      server.joinLoop();
  } 
  else 
  {
    //If the server is not running, displaying an error code and terminating the program
      std::cout<<"Server start error! Error code:"<< int(server.getStatus()) <<std::endl;
      return -1;
  }
  return 0;
}



