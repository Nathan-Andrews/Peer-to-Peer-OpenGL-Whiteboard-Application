#include "Source.h"

ClientSession::ClientSession(CONNECTION_PTR client) : clientConnection(client), openPort(0) {
    // std::thread(&ClientSession::sessionThreadFunction, this).detach();
}

bool ClientSession::waitForMessage() {
    try {
        std::string message = clientConnection->read();
        openPort = std::stoi(message);
        std::cout << "recieved new client port: " << openPort << std::endl;

        return true;
    }
    catch (...) {
        return false;
    }
}

Server::Server(int port) {
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        tcp::socket socket(io_context);

        acceptor.accept(socket);
        std::cout << "New client connected!" << std::endl;

        std::thread(&Server::handleClientThreadFunction, this, std::move(socket)).detach();
    }
}

void Server::handleClientThreadFunction(tcp::socket socket) {
    auto client = std::make_shared<Connection>(std::move(socket));

    ClientSession* clientSession = new ClientSession(client);

    {
        std::lock_guard<std::mutex> lock(clients_mutex);

        for (ClientSession* itr : clients) {
            if (itr->getPort() == 0) continue;

            client->write(std::to_string(itr->getPort()));
        }

        clients.push_back(clientSession);
    }
    
    while (clientSession->waitForMessage()) {}

    std::cout << "client disconnected" << std::endl;

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSession), clients.end());
    }
}