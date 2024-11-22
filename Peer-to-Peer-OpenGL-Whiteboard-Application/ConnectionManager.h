#include "Source.h"

class ConnectionManager {
    Connection hostConnection;

    ConcurrentContainer<Connection*> connections;

    Connection* openConnection;

    ConcurrentContainer<std::string> messageBuffer;

    public:
        ConnectionManager();


        std::string readNext();
        void send(std::string);

    private:
        // void addToBuffer(std::string);
        // void handleRead();
        void addConnection(Connection*);

        void connectionThreadFunction(Connection*);
        void serverCommunicationThreadFunction();
        void acceptNewConnectionsThreadFunction();
};