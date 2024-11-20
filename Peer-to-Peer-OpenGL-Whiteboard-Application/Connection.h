#include "Source.h"

using boost::asio::ip::tcp;

class Connection {
    protected:
        boost::asio::io_context io_context;
        tcp::socket socket;

    public:
        // ip, port
        Connection(std::string, std::string);
        // port
        Connection(std::string);
        Connection(tcp::socket socket);


        void read();
        void write();
};