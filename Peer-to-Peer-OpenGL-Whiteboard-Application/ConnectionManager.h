#include "Source.h"

class ConnectionManager {
    Connection hostConnection;

    ConcurrentContainer<Connection*> connections;

    Connection* openConnection;

    ConcurrentContainer<std::string> messageBuffer;

    public:
        ConnectionManager();


        std::string Read();
        void Write(std::string);


        bool isConnected = true;

    private:
        void AddConnection(Connection*);

        void ConnectionThreadFunction(Connection*);
        void ServerCommunicationThreadFunction();
        void AcceptNewConnectionsThreadFunction();
};