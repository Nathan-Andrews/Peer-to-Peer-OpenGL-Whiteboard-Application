#include "Source.h"

// Constructor: Initializes the ConnectionManager by connecting to the host server and setting up threads.
ConnectionManager::ConnectionManager(IP ip, PORT port) 
    : hostConnection(ip, port) {
    // Start a thread for communication with the host server.
    std::thread(&ConnectionManager::ServerCommunicationThreadFunction, this).detach();

    // TODO: Request the current state of the board.

    // Start a thread to accept new connections from other peers.
    std::thread(&ConnectionManager::AcceptNewConnectionsThreadFunction, this).detach();
}

ConnectionManager::ConnectionManager(Host host) : ConnectionManager(host.ip, host.port) {}

// Adds a new connection to the connection list and spawns a thread to handle its messages.
void ConnectionManager::AddConnection(Connection* connection) {
    connections.Add(connection);

    std::cout << "added connection" << std::endl;

    Whiteboard* activeWhiteboard = getActiveWhiteboard();
    if (activeWhiteboard && activeWhiteboard->prevActions.size()){
        cout << "Active whiteboard size: " << activeWhiteboard->prevActions.size() << std::endl;
        for (DrawAction& action : activeWhiteboard->prevActions){
            const std::string serialized = action.serialize();

            if (serialized.size()){
                connection->Write(serialized);
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // work around so reader knows when message starts/stops
            }else{
                std::cout << "empty serialized value... " << std::endl;
            }
        }
    }

    // Create a new thread to manage communication with this connection.
    std::thread(&ConnectionManager::ConnectionThreadFunction, this, connection).detach();
}

// Handles a single connection, reading messages and adding them to the message buffer.
// If the connection closes, removes it from the list and cleans up resources.
void ConnectionManager::ConnectionThreadFunction(Connection* connection) {
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
    // Iterate through all connections and send the message.
    connections.Iterate([message](Connection* connection) {
        connection->Write(message);
    });
}

// Receives the next message from any peer and stores it in a character buffer.
void ConnectionManager::Read(char* buf) {
    std::string data = Read();
    std::strcpy(buf, data.c_str());
}

// Broadcasts a message from a character buffer to all connected peers.
void ConnectionManager::Write(char* buf) {
    Write(std::string(buf));
}