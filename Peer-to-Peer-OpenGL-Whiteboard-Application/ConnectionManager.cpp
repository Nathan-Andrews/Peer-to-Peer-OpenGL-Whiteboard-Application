#include "Source.h"


// Modified constructor - doesn't automatically connect
ConnectionManager::ConnectionManager(IP ip) : ip{ip} {}

bool ConnectionManager::ConnectToPeer(PORT port) {
    try {
        Connection* connection = new Connection(ip, port, true);
        // Start a read thread but DON'T add to connections list
        std::thread(&ConnectionManager::ConnectionThreadFunction, this, connection).detach();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to connect to peer on port " << port << ": " << e.what() << std::endl;
        return false;
    }
}

bool ConnectionManager::StartListening(PORT port) {
    try {
        openConnection = new Connection(ip, port, false);
        std::cout << "Listening on port " << port << std::endl;
        // After accepting connection, add it to broadcast list
        connections.Add(openConnection);  // This one SHOULD be in the list for broadcasting
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to start listening on port " << port << ": " << e.what() << std::endl;
        delete openConnection;
        openConnection = nullptr;
        return false;
    }
}

void ConnectionManager::AddConnection(Connection* connection) {
    //std::cout << "Before adding - connections size: " << connections.Size() << std::endl;
    connections.Add(connection);
    //std::cout << "After adding - connections size: " << connections.Size() << std::endl;

    //std::cout << "Creating thread for connection on port " << connection->GetPort() << std::endl;
    std::thread([this, connection]() {
        ConnectionThreadFunction(connection);
    }).detach();
}


// Handles a single connection, reading messages and adding them to the message buffer.
// If the connection closes, removes it from the list and cleans up resources.
void ConnectionManager::ConnectionThreadFunction(Connection* connection) {
    connection->io_context.run();
    while (true) {
        // Read a message from the connection.
        std::string message = connection->Read();

        // Break the loop if the message is empty (connection closed).
        if (message == "") break;

        // Add the message to the buffer for processing.
        messageBuffer.Add(message);
    }

    // Remove the connection and clean up resources after disconnection.
    connections.Remove(connection);
    std::cout << "peer " << connection->GetPort() << " disconnected\n";
    delete connection;
}

// Thread function to handle communication with the host server.
// Receives peer ports from the server and manages connections to those peers.
void ConnectionManager::ServerCommunicationThreadFunction() {
    while (true) {
        // Read a message from the server (expected to contain a port number).
        std::string message = hostConnection.Read();

        // Break the loop if the server closes the connection.
        if (message == "") break;

        // Connect to the peer at the provided address and port.
        Connection* connection = new Connection(Host(message));

        // Add the new peer connection to the connection list.
        AddConnection(connection);
    }

    std::cout << "disconnected from server" << std::endl;

    // Disconnect from all peers after the server closes.
    while (connections.Size() > 0) {
        Connection* connection = connections.Take();
        connection->Close();
    }

    isConnected = false;
}

// Thread function to accept incoming connections from other peers.
void ConnectionManager::AcceptNewConnectionsThreadFunction() {
    while (true) {
        // Create a socket for another client to connect to.
        openConnection = new Connection();

        // Notify the host server of the open port for new connections.
        std::cout << " waiting on port " << openConnection->GetPort() << std::endl;
        hostConnection.Write(Server::MakeHostPacket(Server::FindIP(),openConnection->GetPort()));

        // Wait for a client to connect.
        openConnection->AcceptConnection();

        // Add the new connection to the list.
        AddConnection(openConnection);
    }
}

// Receives the next message from any connected peer by taking it from the message buffer.
std::string ConnectionManager::Read() {
    return messageBuffer.Take();
}

// Broadcasts a message to all connected peers.
void ConnectionManager::Write(std::string message) {
    //std::cout << "Broadcasting message of size: " << message.size() << std::endl;
    //std::cout << "Current connections: " << connections.Size() << std::endl;

    size_t sent = 0;
    connections.Iterate([&sent, message](Connection* connection) {
        try {
            //std::cout << "Attempting to send to connection " << connection->GetPort() << std::endl;
            connection->Write(message);
            sent++;
            //std::cout << "Successfully sent to connection " << connection->GetPort() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to send to connection " << connection->GetPort() 
                     << ": " << e.what() << std::endl;
        }
    });
   //std::cout << "Sent to " << sent << " connections" << std::endl;
}

// Receives the next message from any peer and stores it in a character buffer.
void ConnectionManager::Read(char* buf) {
    std::string data = Read();
    //std::cout << "read " << data << std::endl;

    std::strcpy(buf, data.c_str());
}

// Broadcasts a message from a character buffer to all connected peers.
void ConnectionManager::Write(char* buf) {
    Write(std::string(buf));
}

ConnectionManager* manager = nullptr;

void createConnectionManager(IP ip){
    if (manager == nullptr){
        manager = new ConnectionManager(ip);
       // manager->AcceptNewConnectionsThreadFunction();
    }
}

ConnectionManager* getConnectionManager(){
    return manager;
}