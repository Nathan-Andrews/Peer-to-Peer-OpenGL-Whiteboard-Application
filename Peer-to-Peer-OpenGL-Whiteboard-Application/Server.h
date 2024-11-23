#include "Source.h"

// the Host class holds an IP and a port
struct Host {
    IP ip;
    PORT port;

    // constructor: to parse string
    Host(std::string message) {
        try {
            // message format "<ip_address> <port>"
            std::stringstream ss(message); 
            // Set the LAN address.  Expected to be an IP
            std::getline(ss, ip, ' ');

            // Convert the message into an integer (expected to be a port number).
            std::string temp;
            std::getline(ss, temp);
            port = std::stoi(temp);
        } catch (...) {
            throw "packet format error";
        }
    }

    // constructor: default
    Host() {
        ip = "";
        port = 0;
    }
};

// The ClientSession class manages communication with an individual client.
class ClientSession {
    friend class Server; // Grant the Server class access to private members of ClientSession.
    
    Connection* clientConnection; // Represents the connection associated with the client.
    Host host; // The open port number received from the client.

    // Constructor: Initializes the client session with a given connection.
    ClientSession(Connection*);

    // Waits for a message from the client, processes it, and returns true if successful.
    // Returns false if an error occurs or the connection is closed.
    bool WaitForMessage();

    public:
        // Returns the open port associated with this client session.
        PORT GetPort() { return host.port; }
        // returns the LAN ip of the client
        IP GetIP() { return host.ip; }
};

// The Server class manages multiple client connections and facilitates communication between them.
class Server {
    boost::asio::io_context io_context; // I/O context for managing asynchronous operations.
    std::vector<ClientSession*> clients; // A list of active client sessions.

    std::mutex clients_mutex; // Mutex to ensure thread-safe access to the clients list.

    Host host;

public:
    // Constructor: Initializes the server to listen on an arbitrary port, or a specific port.
    // Made to be used with an arbitrary port.  Only use a specific port during testing
    Server(PORT _port = 0);

    // 
    PORT GetPort() {return host.port;}
    IP GetIP() {return host.ip;}

    // finds a LAN address of the computer
    static IP FindIP() {
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

    // Makes a packet to send host information between client and server 
    static std::string MakeHostPacket(IP ip, PORT port) {
        return ip + " " + std::to_string(port);
    }

    static std::string MakeHostPacket(Host host) {
        return MakeHostPacket(host.ip,host.port);
    }


private:
    // Handles communication with a single client in a separate thread.
    // Manages the client's lifecycle and ensures proper interaction with other clients.
    void HandleClientThreadFunction(tcp::socket socket);
};
