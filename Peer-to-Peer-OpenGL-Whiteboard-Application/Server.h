#include "Source.h"

class Server {
    boost::asio::io_context io_context;
    std::vector<std::shared_ptr<Connection>> clients;

    public:
        Server(std::string);

        void HandleInput();
};