#include "Source.h"

class ConnectionManager {
    // Connection hostConnection;

    std::vector<Connection*> connections;

    ConcurrentQueue<std::string> messageBuffer;

    public:
        ConnectionManager();

        void addConnection(Connection*);

        std::string readNext();
        void send(std::string);

    private:
        // void addToBuffer(std::string);
        // void handleRead();

        void connectionThreadFunction(Connection*);
};