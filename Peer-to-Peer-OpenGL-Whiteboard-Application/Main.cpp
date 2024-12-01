#include "Source.h"

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

std::mutex mtx;
std::condition_variable wait_form;
// std::condition_variable wait_label;
FormData* formData = new FormData();

void serverThreadFunction() {
   Server server(11111);
//    std::cout << "server running on "
//       << server.GetIP()
//       << ":"
//       << server.GetPort()
//       << std::endl;
    {
        std::unique_lock<std::mutex> lock(mtx);

        formData->host = server.GetHost();
        formData->type = SESSION_LINK;

        wait_form.notify_all();
    }

    server.Listen();
}

void dataThreadFunction() {
    std::cout << "waiting...\n";
    {
        std::unique_lock<std::mutex> lock(mtx);
        wait_form.wait(lock, [] { return formData->type != NONE; });
        std::cout << "done waiting\n";

        if (formData->type == HOST_SERVER) {
            std::thread(serverThreadFunction).detach();
        }
    }
}


int main(int argc, char* argv[]) {
    std::thread(dataThreadFunction).detach();

    QApplication a(argc, argv);
    
    // Create an instance of the ClientInterface class (the main window).
    ClientInterface w(mtx,wait_form,formData,nullptr);

    // Display the main window by calling the 'show()' method.
    w.show();

    // The call to 'a.exec()' starts the Qt application event loop.
    a.exec();

    if (formData->type != JOIN_SERVER) return 0;

    ConnectionManager* manager = new ConnectionManager(LOCALHOST,11111);

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
    whiteboard.setConnectionManager(manager);
    
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