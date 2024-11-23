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

    HOST host;  // the LAN address of the server
    PORT port;  // the port of the server

public:
    // Constructor: Initializes the server to listen on an arbitrary port.
    Server();

    // 
    PORT GetPort() {return port;}
    HOST GetHost() {return host;}

    // finds a LAN address of the computer
    static HOST FindHostname() {
        boost::asio::io_context io_context;

        // Get the host name
        std::string hostname = boost::asio::ip::host_name();

        // Resolve host name to IP address
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(hostname, "");

        // Iterate through results to find LAN IP address
        for (const auto& entry : endpoints) {
            auto addr = entry.endpoint().address();
            // Only consider IPv4 non-loopback addresses
            if (addr.is_v4() && !addr.is_loopback()) {
                return addr.to_string();
            }
        }

        throw "hostname not found";
    }


private:
    // Handles communication with a single client in a separate thread.
    // Manages the client's lifecycle and ensures proper interaction with other clients.
    void HandleClientThreadFunction(tcp::socket socket);
};
