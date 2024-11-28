#include "Source.h"
#include "Whiteboard.h"

int main() {
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
    bool check = whiteboard.addDrawAction("skibidi", 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, { 0.0f,0.0f,100.0f,100.0f });

    // whiteboard class already handles your own user actions
    

    // you can use the q button to close the window
    //use [, ] to change brush size
    // use up and down arrows to change transparency
    // use 1, 2, 3 and 4 keys to change the color

    while (!glfwWindowShouldClose(window)) {
        whiteboard.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}