#include "Source.h"

ConnectionManager::ConnectionManager() : hostConnection(LOCALHOST,"11111") {
    // connect to host server

    // ask for all peers
    // connect with all peers
    std::thread(&ConnectionManager::serverCommunicationThreadFunction, this).detach();

    // ask for current board

    // send host details on server
    std::thread(&ConnectionManager::acceptNewConnectionsThreadFunction, this).detach();
}

void ConnectionManager::addConnection(Connection* connection) {
    connections.Add(connection);

    std::cout << "added connection" << std::endl;

    std::thread(&ConnectionManager::connectionThreadFunction,this,connection).detach();
}

void ConnectionManager::connectionThreadFunction(Connection* connection) {
    std::cout << "starting connection thread" << std::endl;

    while (true) {
        messageBuffer.Add(connection->read());
    }
}

// thread to communicate with the server
// server will send ports of users we should connect to
// then we wait to detect when the server closes
void ConnectionManager::serverCommunicationThreadFunction() {
    while (true) {
        std::string message = hostConnection.read();

        if (message == "") break;

        Connection* connection = new Connection(LOCALHOST,message);

        addConnection(connection);
    }

    std::cout << "disconnected from server" << std::endl;
}

void ConnectionManager::acceptNewConnectionsThreadFunction() {
    while (true) {
        // create socket for another client to connect to
        openConnection = new Connection();

        // send open port to host server
        std::cout << "waiting on port " << openConnection->getPort() << std::endl;
        hostConnection.write(openConnection->getPort());

        // wait for another client to connect
        openConnection->waitForConnection();

        // add to the list of open connections
        addConnection(openConnection);
    }
}

std::string ConnectionManager::readNext() {
    return messageBuffer.Take();
}

void ConnectionManager::send(std::string message) {
    connections.Iterate([message](Connection* connection) {
        std::cout << "sending message\n";
        connection->write(message);
    });
}