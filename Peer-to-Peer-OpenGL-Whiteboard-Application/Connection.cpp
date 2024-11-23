#include "Source.h"

std::string Connection::Read() {
    try {
        std::string buffer(MAX_BUFFER_SIZE, '\0');

        std::size_t length = socket.read_some(boost::asio::buffer(buffer));

        buffer.resize(length);

        return buffer;
    } catch (const std::exception& e) {
    }

    return "";
}

void Connection::Write(std::string message) {
    boost::asio::write(socket, boost::asio::buffer(message));
}

Connection::Connection(const std::string ip, const std::string port) : socket(io_context), port(port) { 
    try {
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(ip, port);
        
        boost::asio::connect(socket, endpoints);
        std::cout << "Connected to the server!" << std::endl;
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

Connection::Connection(std::string port) : socket(io_context), port(port) {
    acceptor = boost::asio::ip::tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), std::stoi(port)));
}

Connection::Connection() : socket(io_context), acceptor(io_context) {
    try {
        // Specify the endpoint with port 0 for an arbitrary port
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 0);

        // Open the acceptor with the specified endpoint
        GetAcceptor().open(endpoint.protocol());
        GetAcceptor().bind(endpoint);

        port = std::to_string(GetAcceptor().local_endpoint().port());
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

Connection::Connection(tcp::socket socket) : socket(std::move(socket)) {};

void Connection::WaitForConnection() {
    // Start listening for incoming connections
    GetAcceptor().listen();

    std::cout << "Waiting for connection..." << std::endl;

    GetAcceptor().accept(socket);
    std::cout << "Connection established!" << std::endl;
}

void Connection::Close() {
    socket.close();
}

std::string Connection::GetPort() {
    return port;
}

boost::asio::ip::tcp::acceptor& Connection::GetAcceptor() {
    if (!acceptor.has_value()) throw "acceptor is null!";

    return acceptor.value();
}