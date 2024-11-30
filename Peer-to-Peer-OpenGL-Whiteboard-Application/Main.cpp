#include "Source.h"
#include "Whiteboard.h"
#include "ConnectionManager.h"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

class DrawActionQueue {
private:
    std::queue<DrawAction> actions;
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<bool> should_terminate{false};

public:
    void push(const DrawAction& action) {
        std::lock_guard<std::mutex> lock(mutex);
        actions.push(action);
        cv.notify_one();
    }

    bool pop(DrawAction& action) {
        std::lock_guard<std::mutex> lock(mutex);
        if (actions.empty()) {
            return false;
        }
        action = actions.front();
        actions.pop();
        return true;
    }

    void terminate() {
        should_terminate = true;
        cv.notify_all();
    }

    bool isTerminated() {
        return should_terminate;
    }
};

DrawActionQueue actionQueue;

int main(int argc, char* argv[]) {
    //these probably all coulda gone into whiteboard but nahhh
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Whiteboard", NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return -1;

    //put the username that represents you.
    //(maybe we need an actual id system)
    Whiteboard whiteboard("me!");
    whiteboard.setWindow(window);

    // example of adding other user actions
    // this should draw a line from the top right to the middleish
    //bool check = whiteboard.addDrawAction("skibidi", 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, { 0.0f,0.0f,100.0f,100.0f });

    // whiteboard class already handles your own user actions
    //if (argc != 2) return 1;

    Connection::StartIOContext();
    createConnectionManager("127.0.0.1");
    ConnectionManager* manager = getConnectionManager();
    
    std::thread networkThread([window, manager]() {
        while (!glfwWindowShouldClose(window) && !actionQueue.isTerminated()) {
            try {
                // Get the raw message as a string first
                std::string message = manager->Read();
                if (message.empty()) {
                    continue;
                }

                // Deserialize the string directly
                DrawAction action = DrawAction::deserialize(message);
                actionQueue.push(action);
            } catch (const std::exception& e) {
                std::cerr << "Network thread error: " << e.what() << std::endl;
                // Continue listening instead of crashing
                continue;
            }
        }
    });

    std::thread commandThread([manager, window](){
        while (!glfwWindowShouldClose(window) && !actionQueue.isTerminated()){
            std::string command;
            std::cout << "Enter command (listen <port> or connect <port>): ";
            std::cin >> command;
            
            if (command == "listen") {
                PORT port;
                std::cin >> port;
                if (manager->StartListening(port)) {
                    std::cout << "Now listening on port " << port << std::endl;
                }
            }
            else if (command == "connect") {
                PORT port;
                std::cin >> port;
                if (manager->ConnectToPeer(port)) {
                    std::cout << "Connected to peer on port " << port << std::endl;
                }
            }
        }
    });

    // you can use the q button to close the window
    //use [, ] to change brush size
    // use up and down arrows to change transparency
    // use 1, 2, 3 and 4 keys to change the color

    while (!glfwWindowShouldClose(window)) {
        DrawAction action;
        while (actionQueue.pop(action)) {
            std::cout << "added draw action" << std::endl;
            whiteboard.addDrawAction(action);
        }

        whiteboard.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}