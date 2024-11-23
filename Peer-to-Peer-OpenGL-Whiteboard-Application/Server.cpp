#include "Source.h"

// Constructor for initializing a ClientSession with a given Connection.
ClientSession::ClientSession(Connection* client) 
    : clientConnection(client), openPort(0) {}

// Waits for a message from the client, parsing it to obtain an open port.
// Returns true if a message is successfully received and processed, false otherwise.
bool ClientSession::WaitForMessage() {
    try {
        // Read a message from the client connection.
        std::string message = clientConnection->Read();

        // Convert the message into an integer (expected to be a port number).
        openPort = std::stoi(message);
        std::cout << "Received new client port: " << openPort << std::endl;

        return true; // Successfully received and processed a message.
    } catch (...) {
        return false; // An error occurred (e.g., invalid data or connection issue).
    }
}

// Constructor for initializing the server to listen on a specific port.
// Accepts client connections in a loop, spawning a new thread for each client.
Server::Server(PORT port) {
    // Create a TCP acceptor to listen on the specified port.
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    std::cout << "Server listening on port " << port << std::endl;

    // Infinite loop to accept incoming client connections.
    while (true) {
        tcp::socket socket(io_context); // Create a socket for the incoming connection.

        // Accept a new client connection.
        acceptor.accept(socket);
        std::cout << "New client connected!" << std::endl;

        // Spawn a new thread to handle the client.
        std::thread(&Server::HandleClientThreadFunction, this, std::move(socket)).detach();
    }
}

// Function to handle interactions with a single client.
// This function is run in a separate thread for each client.
void Server::HandleClientThreadFunction(tcp::socket socket) {
    // Create a Connection object to manage the client socket.
    Connection* client = new Connection(std::move(socket));

    // Create a ClientSession to manage communication with the client.
    ClientSession* clientSession = new ClientSession(client);

    {
        // Lock the clients list for thread-safe access.
        std::lock_guard<std::mutex> lock(clients_mutex);

        // Send the client's open port to all other connected clients.
        for (ClientSession* itr : clients) {
            if (itr->GetPort() == 0) continue; // Skip clients without a valid port.

            client->Write(std::to_string(itr->GetPort())); // Send the port.
        }

        // Add the new client session to the list of active clients.
        clients.push_back(clientSession);
    }

    // Continuously wait for messages from the client until it disconnects.
    while (clientSession->WaitForMessage()) {}

    // Client disconnected; log the event.
    std::cout << "Client disconnected" << std::endl;

    {
        // Lock the clients list for thread-safe modification.
        std::lock_guard<std::mutex> lock(clients_mutex);

        // Remove the disconnected client session from the list.
        clients.erase(std::remove(clients.begin(), clients.end(), clientSession), clients.end());
    }
}
