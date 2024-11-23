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

        PORT port;

        // ip, port
        Connection(HOST, PORT);
        // port
        [[deprecated("Use Connection() instead, which assigns an arbitrary port")]]
        Connection(PORT);
        Connection();
        Connection(tcp::socket socket);


        std::string Read();
        void Write(std::string);

        void Close();

        void WaitForConnection();

        PORT GetPort();
    
        boost::asio::ip::tcp::acceptor& GetAcceptor();
};