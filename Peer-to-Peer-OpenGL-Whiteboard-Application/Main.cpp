#include "Source.h"

using boost::asio::ip::tcp;


int inputThread(ConnectionManager& manager) {
    std::string message;

    while (true) {
        std::getline(std::cin, message);
        
        manager.send(message);
    }
}

int main() {
    try {
        std::cout << "Enter mode (1 = server, 2 = client): ";
        int mode;
        std::cin >> mode;
        std::cin.ignore(); // Clear the newline character

        ConnectionManager manager;

        if (mode == 1) {
            // Server mode
            // Server("12345");
            Connection* connection = new Connection("12345");
            manager.addConnection(connection);

        } else if (mode == 2) {
            // Client mode
            Connection* connection = new Connection("127.0.0.1","12345");
            manager.addConnection(connection);
        } else {
            std::cerr << "Invalid mode. Exiting." << std::endl;
        }


        std::thread(inputThread,std::ref(manager)).detach();
        while (true) {
            std::string message = manager.readNext();
            std::cout << "recieved: " << message << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}