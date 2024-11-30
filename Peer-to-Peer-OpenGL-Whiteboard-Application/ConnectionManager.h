#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include "Source.h"

// The ConnectionManager class manages connections to both a central server and peers.
// It handles sending and receiving messages, managing peer connections, and communicating with the server.
class ConnectionManager {
    IP ip;

    // Connection to the central host server.
    Connection hostConnection;

    // Container to store connections to peers in a thread-safe manner.
    ConcurrentContainer<Connection*> connections;

    // Pointer to the current open connection (awaiting a new peer connection).
    Connection* openConnection;

    // Thread-safe container to buffer messages received from peers.
    ConcurrentContainer<std::string> messageBuffer;

    public:
        bool ConnectToPeer(PORT port);
        bool StartListening(PORT port);

        // Constructor to initialize the ConnectionManager with a host and port.
        ConnectionManager(IP ip);

        // Reads the next message from any connected peer.
        std::string Read();

        // Broadcasts a message to all connected peers.
        void Write(std::string);

        // Reads the next message into a character buffer.
        void Read(char* buf);

        // Broadcasts a message from a character buffer to all connected peers.
        void Write(char* buf);

        // Indicates whether the ConnectionManager is still connected to the server.
        bool isConnected = true;

        // Manages communication with the central server.
        // Receives peer information and handles disconnection from the server.
        void ServerCommunicationThreadFunction();

    private:
        // Adds a new connection to the peer list and starts a thread to handle it.
        void AddConnection(Connection*);

        // Manages communication with a single connection.
        void ConnectionThreadFunction(Connection*);

        // Listens for and accepts incoming peer connections.
        void AcceptNewConnectionsThreadFunction();
};

ConnectionManager* getConnectionManager();
void createConnectionManager(IP);

#endif