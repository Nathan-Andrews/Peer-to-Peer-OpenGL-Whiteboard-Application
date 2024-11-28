#include "Source.h"
#include "Whiteboard.h"

int main() {
    //these probably all coulda gone into whiteboard but nahhh
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Whiteboard", NULL, NULL);
    if (!window) return -1;
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) return -1;

    Whiteboard whiteboard;
    whiteboard.setWindow(window);

    // example of adding other user actions
    // this should draw a line from the top right to the middleish
    bool check = whiteboard.addDrawAction("skibidi", 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, { 0.0f,0.0f,100.0f,100.0f });

    // whiteboard class already handles your own user actions
    
    while (!glfwWindowShouldClose(window)) {
        whiteboard.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}