#include "Source.h"

using boost::asio::ip::tcp;

// The Connection class represents a network connection that can act as either 
// a client (outgoing connection) or a server (accepting incoming connections).
class Connection {
    // Declare friend classes to allow specific classes access to Connection's private members.
    friend class Server;
    friend class ClientSession;
    friend class ConnectionManager;

private:
    boost::asio::io_context io_context; // The I/O context for managing asynchronous operations.
    tcp::socket socket;                // The socket used for communication.
    std::optional<tcp::acceptor> acceptor; // Optional acceptor for managing incoming connections (server mode).

    PORT port; // The port associated with the connection or acceptor.

    // Constructor for initializing a client connection to a remote server.
    Connection(HOST ip, PORT port);

    // Constructor for initializing a server with a specific port.
    // Marked as deprecated because a constructor with an arbitrary port is preferred.
    [[deprecated("Use Connection() instead, which assigns an arbitrary port")]]
    Connection(PORT port);

    // Default constructor for initializing a server with an arbitrary port.
    Connection();

    // Constructor for wrapping an existing socket into a Connection object.
    Connection(tcp::socket socket);

    // Reads data from the socket and returns it as a string.
    std::string Read();

    // Writes the provided message to the socket.
    void Write(std::string message);

    // Closes the socket, terminating the connection.
    void Close();

    // Waits for an incoming connection and assigns it to the socket.
    void AcceptConnection();

    // Returns the port associated with the connection or the acceptor.
    PORT GetPort();
    
    // Provides access to the TCP acceptor. Throws an error if the acceptor is not initialized.
    boost::asio::ip::tcp::acceptor& GetAcceptor();
};
