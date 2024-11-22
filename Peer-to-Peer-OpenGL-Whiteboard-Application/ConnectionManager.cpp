#include "Source.h"

ConnectionManager::ConnectionManager() : hostConnection("127.0.0.1","12345") {
    // connect to host server

    std::cout << hostConnection.read() << std::endl;
    hostConnection.write("11100");
    // ask for all peers

    // connect with all peers

    // ask for current board

    // send host details on server
    // throw "method not implemented";
}

void ConnectionManager::addConnection(Connection* connection) {
    connections.push_back(connection);

    std::cout << "added connection" << std::endl;

    std::thread(&ConnectionManager::connectionThreadFunction,this,connection).detach();
}

void ConnectionManager::connectionThreadFunction(Connection* connection) {
    std::cout << "starting connection thread" << std::endl;

    while (true) {
        messageBuffer.Add(connection->read());
    }
}

std::string ConnectionManager::readNext() {
    return messageBuffer.Take();
}

void ConnectionManager::send(std::string message) {
    for (Connection* connection : connections) {
        connection->write(message);
    }
}