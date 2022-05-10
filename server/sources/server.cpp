#include "../headers/server.h"

//The constructor accepts:
//port - the port on which we will run the server
//handler - callback function to run when the client connects
TcpServer::TcpServer(const uint16_t port, handler_function_t handler) : port(port), handler(handler) {}


TcpServer::~TcpServer() 
{
    if(_status == status::up)
    stop();
}
 

//Set a callback function to run when a client connects
void TcpServer::setHandler(TcpServer::handler_function_t handler)
{
    this->handler = handler;
}


uint16_t TcpServer::getPort() const
{
    return port;
}


uint16_t TcpServer::setPort( const uint16_t port) 
{
    this->port = port;
    restart(); 
    return port;
}


//server restart
TcpServer::status TcpServer::restart() 
{
    if(_status == status::up)
    stop ();
    return start();
}


// Entering the Connection Handling Thread
void TcpServer::joinLoop()
{
    handler_thread.join();
}


//Server start
TcpServer::status TcpServer::start() 
{
    struct sockaddr_in server;
    server.sin_addr.s_addr =  inet_addr(m_ipAddress.c_str());
    server.sin_port = htons(port);
    server.sin_family = AF_INET;
    serv_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(serv_socket == -1) 
    {
        return _status = status::err_socket_init;
    }

    if (setsockopt (serv_socket, SOL_SOCKET, SO_KEEPALIVE, &m_keepalive, sizeof(m_keepalive)) < 0)    
    {           
        perror("\nSet keepalive error:\n");   
    }  
    else 
        std::cout<<"\nSet keepalive successfully!!!";

    if(bind(serv_socket,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
         return _status = status::err_socket_bind;
    }

    if(listen(serv_socket, 3) < 0)
    {
        return _status = status::err_socket_listening;
    }

    _status = status::up;
    handler_thread = std::thread([this]{handlingLoop();});
    return _status;
}


//Server stop
void TcpServer::stop() 
{
    _status = status::close;
    close(serv_socket);
    joinLoop();
    for(std::thread& client_thr : client_handler_threads)
    {
        client_thr.join();
    }
    ClientsList.clear();
    client_handler_threads.clear ();
    client_handling_end.clear ();
}


// Connection Handling Function
void TcpServer::handlingLoop() 
{
    while (_status == status::up) 
    {
        int client_socket;
        struct sockaddr_in client_addr;
        int addrlen = sizeof (struct sockaddr_in);
        client_socket = accept(serv_socket, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
        setsockopt (client_socket, SOL_SOCKET, SO_KEEPALIVE, &m_keepalive, sizeof(m_keepalive));
        if(client_socket >= 0 && _status == status::up)
        {
            client_handler_threads.push_back(std::thread([this, &client_socket, &client_addr] {
            handler(Client(client_socket, client_addr));
            ClientsList.emplace(std::this_thread::get_id(),client_socket);
            client_handling_end.push_back (std::this_thread::get_id());
            }));
            TcpServer::socketsVector.push_back(client_socket);
        }
        if(!client_handling_end.empty())
          for(std::list<std::thread::id>::iterator id_it = client_handling_end.begin (); !client_handling_end.empty() ; id_it = client_handling_end.begin())
            for(std::list<std::thread>::iterator thr_it = client_handler_threads.begin (); thr_it != client_handler_threads.end () ; ++thr_it)
                if(thr_it->get_id () == *id_it) 
                {
                    thr_it->join();
                    std::map <std::thread::id, int> :: iterator clientList_itr;
                    clientList_itr = ClientsList.find(*id_it);
                    ClientsList.erase(clientList_itr);
                    client_handler_threads.erase(thr_it);
                    client_handling_end.erase (id_it);
                    break;
                } 

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

}


TcpServer::Client::Client(int socket, struct sockaddr_in address) : socket(socket), address(address) {
}


TcpServer::Client::~Client() 
{
    std::vector<int>::iterator it = TcpServer::socketsVector.begin();
    while(it != TcpServer::socketsVector.end())
    {
        if(*it == socket)
            TcpServer::socketsVector.erase(it);
    }               
    shutdown(socket, 0); 
    close(socket); 
}

bool TcpServer::Client::operator== (const Client  & other) const{
            if(this->socket == other.socket){
                return true;
            }
            return false;
        }
void TcpServer::Client::setName(std::string name)
{
    clientName = name;
}

uint32_t TcpServer::Client::getHost()const 
{
    return address.sin_addr.s_addr;
}


uint16_t TcpServer::Client::getPort()const
{
    return address.sin_port;
}


//checkblock
bool TcpServer::Client::checkBlock(char *str)
{
    char str1 [8]="BLOCKME";
    char *istr;
    istr = strstr (str,str1);
    if ( istr == NULL)
        return false;
    else
        return true;
}


// Regulates the order in which messages are exchanged between the client and the server
void TcpServer::Client::messageExchange(std::string conHost) 
{
    memset(&buffer, 0, sizeof(buffer));
    int number = 0;
    while((number = recv(socket, buffer, sizeof(buffer), 0))>0)
    {
        if(clientName != "")
        {
        std::unique_lock<std::mutex> locker(lockprint);
        std::cout << "Receive from "<< clientName << ": " << buffer << std::endl;
        }
        else
        {
            std::unique_lock<std::mutex> locker(lockprint);
            std::cout << "Receive from "<< conHost << ": " << buffer << std::endl;
        }
        if (strcmp(buffer, "exit") != 0)
        {
           //if (!checkBlock(buffer)){
                std::vector<int>::iterator it = TcpServer::socketsVector.begin();
                while(it != TcpServer::socketsVector.end())
                {
                    if(*it != socket)
                        send(*it, buffer, number, 0);
                }               
          // }
        }
    }
}


//Send message to client
void TcpServer::Client::sendMsg()
{
    puts(buffer);
    if((send(socket, buffer, sizeof(buffer), 0)) < 0)
    {
        perror( "Error send msg to the client!" );
    }
    std::cout<<"Send to client"<<buffer<<std::endl;
}              


// Receive message from client
std::string TcpServer::Client::receiveMsg()
{
     memset(&buffer, 0, sizeof(buffer));
     recv(socket, buffer, sizeof(buffer), 0);
     std::cout << "Receive from client: " << buffer << std::endl;
     std::string check = std::string(buffer);
     return check;
}          