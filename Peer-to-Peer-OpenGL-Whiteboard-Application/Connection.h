#include "Source.h"

using boost::asio::ip::tcp;

class Connection {
    friend class Server;
    friend class ClientSession;
    friend class ConnectionManager;

    private:
        boost::asio::io_context io_context;
        tcp::socket socket;
        std::optional<boost::asio::ip::tcp::acceptor> acceptor;

        std::string port;

        // ip, port
        Connection(std::string, std::string);
        // port
        [[deprecated("Use Connection() instead, which assigns an arbitrary port")]]
        Connection(std::string);
        Connection();
        Connection(tcp::socket socket);


        std::string Read();
        void Write(std::string);

        void WaitForConnection();

        std::string GetPort();
    
        boost::asio::ip::tcp::acceptor& GetAcceptor();
};