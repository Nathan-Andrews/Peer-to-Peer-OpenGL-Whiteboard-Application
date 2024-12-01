#include "Source.h"
#include "Globals.h"

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

void serverThreadFunction() {
   Server server;

    {
        std::unique_lock<std::mutex> lock(form_mtx);

        formData->host = server.GetHost();
        formData->type = SESSION_LINK;

        wait_form.notify_all();
    }

    server.Listen();
}

void formThreadFunction() {
    for (int cont = true; cont;) {
        std::cout << "waiting...\n";
        {
            std::unique_lock<std::mutex> lock(form_mtx);
            wait_form.wait(lock, [] { return formData->type == HOST_SERVER || formData->type == JOIN_SERVER; });
            std::cout << "done waiting\n";

            if (formData->type == HOST_SERVER) {
                std::thread(serverThreadFunction).detach();
                break;
            }
            else if (formData->type == JOIN_SERVER) {
                manager = new ConnectionManager(formData->host);

                sleep(1); // wait for connection

                // connection successful
                if (manager->isConnected) {
                    formData->type = JOIN_SUCCESS;

                    cont = false;
                }
                // connection unsuccessful
                else {
                    delete manager;
                    manager = nullptr;
                    
                    formData->type = JOIN_FAIL;
                }
            }
        }

        wait_form.notify_all();
    }
}


int main(int argc, char* argv[]) {
    std::thread(formThreadFunction).detach();

    QApplication a(argc, argv);
    
    // Create an instance of the ClientInterface class (the main window).
    ClientInterface w(nullptr);

    // Display the main window by calling the 'show()' method.
    w.show();

    // The call to 'a.exec()' starts the Qt application event loop.
    a.exec();

    if (manager == nullptr) return 0;

    //these probably all coulda gone into whiteboard but nahhh
    if (!glfwInit()) exit(-1);
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Whiteboard", NULL, NULL);
    if (!window) exit(-1);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) exit(-1);

    //put the username that represents you.
    //(maybe we need an actual id system)
    Whiteboard whiteboard("me!");
    whiteboard.setWindow(window);
    
    std::thread networkThread([window]() {
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

    // closes the window when the server disconnects
    std::thread windowCloseThread([window]() {
        while (manager->isConnected) sleep(1);
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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