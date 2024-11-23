#include "Source.h"

// The ClientSession class manages communication with an individual client.
class ClientSession {
    friend class Server; // Grant the Server class access to private members of ClientSession.
    
    Connection* clientConnection; // Represents the connection associated with the client.
    int openPort; // The open port number received from the client.

    // Constructor: Initializes the client session with a given connection.
    ClientSession(Connection*);

    // Returns the open port associated with this client session.
    int GetPort() { return openPort; }

    // Waits for a message from the client, processes it, and returns true if successful.
    // Returns false if an error occurs or the connection is closed.
    bool WaitForMessage();
};

// The Server class manages multiple client connections and facilitates communication between them.
class Server {
    boost::asio::io_context io_context; // I/O context for managing asynchronous operations.
    std::vector<ClientSession*> clients; // A list of active client sessions.

    std::mutex clients_mutex; // Mutex to ensure thread-safe access to the clients list.

public:
    // Constructor: Initializes the server to listen on a specified port.
    Server(PORT port);

private:
    // Handles communication with a single client in a separate thread.
    // Manages the client's lifecycle and ensures proper interaction with other clients.
    void HandleClientThreadFunction(tcp::socket socket);
};
