#include "Source.h"

void Connection::read() {
    try {
        char buffer[1024];
        while (true) {
            std::size_t length = socket.read_some(boost::asio::buffer(buffer));
            std::cout << "Peer: " << std::string(buffer, length) << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Read error: " << e.what() << std::endl;
    }
}

void Connection::write() {
    try {
        std::string message;

        while (true) {
            std::getline(std::cin, message);
            boost::asio::write(socket, boost::asio::buffer(message));
        }
    } catch (const std::exception& e) {
        std::cerr << "Write error: " << e.what() << std::endl;
    }
}

Connection::Connection(const std::string ip, const std::string port) : socket(io_context) { 
    try {
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(ip, port);
        
        boost::asio::connect(socket, endpoints);
        std::cout << "Connected to the server!" << std::endl;

        std::thread readThread(&Connection::read, this);
        write();
        readThread.join();
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

    std::thread readThread(&Connection::read, this);
    write();
    readThread.join();
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}