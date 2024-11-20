#include "Source.h"

Server::Server(std::string port) {
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), std::stoi(port)));
    std::cout << "Server listening on port " << port << std::endl;

    std::mutex clients_mutex;

    std::thread(&Server::HandleInput,this).detach();

    while (true) {
        tcp::socket socket(io_context);

        acceptor.accept(socket);
        std::cout << "New client connected!" << std::endl;

        auto client = std::make_shared<Connection>(std::move(socket));
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client);
        }

        client->start();
    }
}


void Server::HandleInput() {
    try {
        std::string message;

        while (true) {
            std::getline(std::cin, message);
            for (const auto& client : clients) {                
                client->write(message);
            }

        }
    } catch (const std::exception& e) {
        std::cerr << "Write error: " << e.what() << std::endl;
    }
}