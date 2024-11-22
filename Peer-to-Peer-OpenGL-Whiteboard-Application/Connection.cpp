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

Connection::Connection(const std::string ip, const std::string port) : socket(io_context) { 
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

Connection::Connection(std::string port) : socket(io_context) {
    try {
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), std::stoi(port)));

        std::cout << "Waiting for connection..." << std::endl;

        acceptor.accept(socket);
        std::cout << "Connection established!" << std::endl;
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

Connection::Connection(tcp::socket socket) : socket(std::move(socket)) {};

int Connection::getUniquePort() {
    try {
        // Create an I/O context
        boost::asio::io_context io_context;

        // Create an endpoint with any available port
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 0);

        // Open and bind the socket to the endpoint
        boost::asio::ip::tcp::acceptor acceptor(io_context);
        acceptor.open(endpoint.protocol());
        acceptor.bind(endpoint);

        // Get the assigned port number
        return acceptor.local_endpoint().port();
    } catch (const std::exception& e) {
        std::cerr << "Error finding available port: " << e.what() << std::endl;
        return -1; // Indicate failure
    }
}