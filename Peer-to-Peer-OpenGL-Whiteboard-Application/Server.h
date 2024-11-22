#include "Source.h"

#define CONNECTION_PTR std::shared_ptr<Connection>
class ClientSession {
    CONNECTION_PTR clientConnection;
    int openPort;
    public:
        ClientSession(CONNECTION_PTR);

        int getPort() {return openPort;};

        bool waitForMessage();
};

class Server {
    boost::asio::io_context io_context;
    std::vector<ClientSession*> clients;

    std::mutex clients_mutex;

    public:
        Server(int);
    
    private:
        void handleClientThreadFunction(tcp::socket);
};
