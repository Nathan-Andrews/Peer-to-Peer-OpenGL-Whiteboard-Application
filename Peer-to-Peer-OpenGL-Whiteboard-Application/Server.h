#include "Source.h"

class ClientSession {
    friend class Server;
    
    Connection* clientConnection;
    int openPort;

    ClientSession(Connection*);

    int GetPort() {return openPort;};

    bool WaitForMessage();
};

class Server {
    boost::asio::io_context io_context;
    std::vector<ClientSession*> clients;

    std::mutex clients_mutex;

    public:
        Server(PORT);
    
    private:
        void HandleClientThreadFunction(tcp::socket);
};
