#include "Source.h"

using boost::asio::ip::tcp;

int main() {
    try {
        std::cout << "Enter mode (1 = server, 2 = client): ";
        int mode;
        std::cin >> mode;
        std::cin.ignore(); // Clear the newline character

        if (mode == 1) {
            // Server mode
            Server("12345");

        } else if (mode == 2) {
            // Client mode
            Connection connection("127.0.0.1","12345");
        } else {
            std::cerr << "Invalid mode. Exiting." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}