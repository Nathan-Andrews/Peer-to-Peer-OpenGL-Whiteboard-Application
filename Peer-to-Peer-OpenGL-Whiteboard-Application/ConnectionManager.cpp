#include "Source.h"

ConnectionManager::ConnectionManager(HOST host,PORT port) : hostConnection(host,port) {
    // connect to host server

    // ask for all peers
    // connect with all peers
    std::thread(&ConnectionManager::ServerCommunicationThreadFunction, this).detach();

    // ask for current board

    // send host details on server
    std::thread(&ConnectionManager::AcceptNewConnectionsThreadFunction, this).detach();
}

void ConnectionManager::AddConnection(Connection* connection) {
    connections.Add(connection);

    std::cout << "added connection" << std::endl;

    std::thread(&ConnectionManager::ConnectionThreadFunction,this,connection).detach();
}

void ConnectionManager::ConnectionThreadFunction(Connection* connection) {
    while (true) {
        std::string message = connection->Read();

        if (message == "") break;

        messageBuffer.Add(message);
    }

    connections.Erase(connection);
    std::cout << "peer " << connection->GetPort() << " disconnected\n";
    delete connection;
}

// thread to communicate with the server
// server will send ports of users we should connect to
// then we wait to detect when the server closes
void ConnectionManager::ServerCommunicationThreadFunction() {
    while (true) {
        std::string message = hostConnection.Read();

        if (message == "") break;

        PORT port = std::stoi(message);

        Connection* connection = new Connection(LOCALHOST,port);

        AddConnection(connection);
    }

    std::cout << "disconnected from server" << std::endl;

    // disconnect from all peers
    while (connections.Size() > 0) {
        Connection* connection = connections.Take();
        connection->Close();
    }

    isConnected = false;
}

void ConnectionManager::AcceptNewConnectionsThreadFunction() {
    while (true) {
        // create socket for another client to connect to
        openConnection = new Connection();

        // send open port to host server
        std::cout << " waiting on port " << openConnection->GetPort() << std::endl;
        hostConnection.Write(std::to_string(openConnection->GetPort()));

        // wait for another client to connect
        openConnection->WaitForConnection();

        // add to the list of open connections
        AddConnection(openConnection);
    }
}

std::string ConnectionManager::Read() {
    return messageBuffer.Take();
}

void ConnectionManager::Write(std::string message) {
    connections.Iterate([message](Connection* connection) {
        connection->Write(message);
    });
}

void ConnectionManager::Read(char* buf) {
    std::string data = Read();
    std::strcpy(buf, data.c_str());
}

void ConnectionManager::Write(char* buf) {
    Write(std::string(buf));
}