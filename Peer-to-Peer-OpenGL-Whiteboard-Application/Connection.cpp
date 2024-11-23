#include "Source.h"

// Reads data from the socket into a buffer and returns it as a string.
std::string Connection::Read() {
    try {
        // Create a buffer of maximum size with null characters
        std::string buffer(MAX_BUFFER_SIZE, '\0');

        // Read data from the socket into the buffer
        std::size_t length = socket.read_some(boost::asio::buffer(buffer));

        // Resize the buffer to match the actual data length read
        buffer.resize(length);

        return buffer; // Return the data as a string
    } catch (const std::exception& e) {
        // Handle any exceptions during the read operation (currently empty)
    }

    return ""; // Return an empty string if an error occurs
}

// Writes the provided message to the socket.
void Connection::Write(std::string message) {
    try {
        // Send the message over the socket
        boost::asio::write(socket, boost::asio::buffer(message));
    } catch (...) {
        // Handle any exceptions during the write operation (currently empty)
    }
}

// Constructor for establishing a connection to a remote server.
Connection::Connection(HOST ip, PORT port) : socket(io_context), port(port) { 
    try {
        // Resolve the host and port to obtain endpoints
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(ip, std::to_string(port));

        // Connect the socket to the first available endpoint
        boost::asio::connect(socket, endpoints);
        std::cout << "Connected to the server!" << std::endl;
    } catch (const boost::system::system_error& e) {
        // Handle connection errors and print the error message
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

// Constructor for initializing a server to listen on a specified port.
Connection::Connection(PORT port) : socket(io_context), port(port) {
    // Initialize an acceptor to bind to the specified port
    acceptor = boost::asio::ip::tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), port));
}

// Default constructor for initializing a server on an arbitrary port.
Connection::Connection() : socket(io_context), acceptor(io_context) {
    try {
        // Create an endpoint with an arbitrary port
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 0);

        // Open and bind the acceptor to the endpoint
        GetAcceptor().open(endpoint.protocol());
        GetAcceptor().bind(endpoint);

        // Retrieve the automatically assigned port number
        port = GetAcceptor().local_endpoint().port();
    } catch (const boost::system::system_error& e) {
        // Handle any exceptions during initialization
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

// Constructor that initializes the Connection object with an existing socket.
Connection::Connection(tcp::socket socket) : socket(std::move(socket)) {}

// Waits for an incoming connection and establishes it.
void Connection::AcceptConnection() {
    // Put the acceptor in a listening state
    GetAcceptor().listen();

    std::cout << "Waiting for connection..." << std::endl;

    // Accept an incoming connection and assign it to the socket
    GetAcceptor().accept(socket);
    std::cout << "Connection established!" << std::endl;
}

// Closes the socket, terminating the connection.
void Connection::Close() {
    socket.close();
}

// Returns the port associated with the connection or acceptor.
PORT Connection::GetPort() {
    return port;
}

// Provides access to the TCP acceptor, ensuring it is valid.
boost::asio::ip::tcp::acceptor& Connection::GetAcceptor() {
    if (!acceptor.has_value()) {
        throw "acceptor is null!"; // Throw an error if the acceptor is not initialized
    }

    return acceptor.value(); // Return the acceptor object
}
