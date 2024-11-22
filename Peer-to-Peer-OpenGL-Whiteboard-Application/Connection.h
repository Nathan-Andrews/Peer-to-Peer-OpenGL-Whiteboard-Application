#include "Source.h"

#define LOCALHOST "127.0.0.1"

using boost::asio::ip::tcp;

class Connection {
    protected:
        boost::asio::io_context io_context;
        tcp::socket socket;
        std::optional<boost::asio::ip::tcp::acceptor> acceptor;

        std::string port;

    public:
        // ip, port
        Connection(std::string, std::string);
        // port
        [[deprecated("Use Connection() instead, which assigns an arbitrary port")]]
        Connection(std::string);
        Connection();
        Connection(tcp::socket socket);


        std::string Read();
        [[deprecated("Use write(std::string) instead.  This method is only for testing")]]
        void Write();
        void Write(std::string);

        void Start();

        void WaitForConnection();

        std::string GetPort();
    
    protected:
        boost::asio::ip::tcp::acceptor& GetAcceptor();
};