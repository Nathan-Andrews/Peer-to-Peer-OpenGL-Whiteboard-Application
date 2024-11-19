#include "Source.h"

using boost::asio::ip::tcp;

class Connection {
    boost::asio::io_context io_context;
    tcp::socket socket;

    public:
        // ip, port
        Connection(std::string, std::string);
        // port
        Connection(std::string);


        void readFromSocket();
        void writeToSocket();
};