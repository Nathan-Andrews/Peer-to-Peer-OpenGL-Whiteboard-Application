#include "Source.h"

int inputThread(ConnectionManager& manager) {
    std::string message;

    while (true) {
        std::getline(std::cin, message);
        
        manager.Write(message);
    }
}

int main() {
    try {
        std::cout << "Enter mode (1 = server, 2 = client): ";
        int mode;
        std::cin >> mode;
        std::cin.ignore(); // Clear the newline character


        if (mode == 1) {
            // Server mode
            Server(11111);
        } else if (mode == 2) {
            // Client mode
            ConnectionManager manager;

            std::thread(inputThread,std::ref(manager)).detach();
            while (true) {
                std::string message = manager.Read();
                std::cout << "recieved: " << message << std::endl;
            }

        } else {
            std::cerr << "Invalid mode. Exiting." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}