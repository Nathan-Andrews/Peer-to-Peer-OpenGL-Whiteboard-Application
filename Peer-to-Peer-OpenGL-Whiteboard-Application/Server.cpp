#include "Source.h"

ClientSession::ClientSession(Connection* client) : clientConnection(client), openPort(0) {}

bool ClientSession::WaitForMessage() {
    try {
        std::string message = clientConnection->Read();
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

        std::thread(&Server::HandleClientThreadFunction, this, std::move(socket)).detach();
    }
}

void Server::HandleClientThreadFunction(tcp::socket socket) {
    Connection* client = new Connection(std::move(socket));

    ClientSession* clientSession = new ClientSession(client);

    {
        std::lock_guard<std::mutex> lock(clients_mutex);

        for (ClientSession* itr : clients) {
            if (itr->GetPort() == 0) continue;

            client->Write(std::to_string(itr->GetPort()));
        }

        clients.push_back(clientSession);
    }
    
    while (clientSession->WaitForMessage()) {}

    std::cout << "client disconnected" << std::endl;

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSession), clients.end());
    }
}