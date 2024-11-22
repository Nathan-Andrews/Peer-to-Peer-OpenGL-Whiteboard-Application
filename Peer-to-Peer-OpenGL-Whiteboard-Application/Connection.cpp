#include "Source.h"

std::string Connection::read() {
    try {
        std::string buffer(1024, '\0');

        std::size_t length = socket.read_some(boost::asio::buffer(buffer));

        buffer.resize(length);

        return buffer;
    } catch (const std::exception& e) {
    }

    return "";
}

void Connection::write() {
    try {
        std::string message;

        while (true) {
            std::getline(std::cin, message);
            boost::asio::write(socket, boost::asio::buffer(message));
        }
    } catch (const std::exception& e) {
    }
}

void Connection::write(std::string message) {
    boost::asio::write(socket, boost::asio::buffer(message));
}

void Connection::start() {
    std::thread readThread(&Connection::read, this);
    write();
    readThread.join();
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
        getAcceptor().open(endpoint.protocol());
        getAcceptor().bind(endpoint);

        port = std::to_string(getAcceptor().local_endpoint().port());
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

Connection::Connection(tcp::socket socket) : socket(std::move(socket)) {};

void Connection::waitForConnection() {
    // Start listening for incoming connections
    getAcceptor().listen();

    std::cout << "Waiting for connection..." << std::endl;

    getAcceptor().accept(socket);
    std::cout << "Connection established!" << std::endl;
}

std::string Connection::getPort() {
    return port;
}

boost::asio::ip::tcp::acceptor& Connection::getAcceptor() {
    if (!acceptor.has_value()) throw "acceptor is null";

    return acceptor.value();
}