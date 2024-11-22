#include "Source.h"

#define LOCALHOST "127.0.0.1"

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


        std::string read();
        void write();
        void write(std::string);

        void start();

        static int getUniquePort();
};