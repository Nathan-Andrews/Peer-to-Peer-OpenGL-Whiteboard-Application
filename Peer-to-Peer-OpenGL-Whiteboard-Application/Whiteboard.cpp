#ifndef WHITEBOARD
#define WHITEBOARD

#include "Source.h"
#include "Globals.h"

// Static members for handling callbacks
Whiteboard* activeWhiteboard = nullptr;

void Whiteboard::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (activeWhiteboard) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                std::cout << "pressed!" << std::endl;
                activeWhiteboard->mouseDownOnWindow = true;

                glfwGetCursorPos(window, &activeWhiteboard->curr_xpos, &activeWhiteboard->curr_ypos);

                int width, height;
                glfwGetWindowSize(window, &width, &height);
                float x = (activeWhiteboard->curr_xpos / width) * 2.0f - 1.0f;
                float y = 1.0f - (activeWhiteboard->curr_ypos / height) * 2.0f;

                activeWhiteboard->vertices.push_back(x);
                activeWhiteboard->vertices.push_back(y);
            }
            else if (action == GLFW_RELEASE) {
                std::cout << "released!" << std::endl;
                activeWhiteboard->mouseDownOnWindow = false;

                if (activeWhiteboard->vertices.size() >= 2) {
                    DrawAction newAction(activeWhiteboard->myId, activeWhiteboard->vertices, activeWhiteboard->brushSize, activeWhiteboard->currentColor);
                    activeWhiteboard->drawActions.push_back(newAction);
                    activeWhiteboard->prevActions.push_back(newAction);

                    if (manager != nullptr){
                        
                        //std::cout << "write draw action" << std::endl;
                        manager->Write(newAction.serialize());
                    }
                }

                activeWhiteboard->vertices.clear();
            }
        }
    }
}

void Whiteboard::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (activeWhiteboard && activeWhiteboard->mouseDownOnWindow) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float x = (xpos / width) * 2.0f - 1.0f;
        float y = 1.0f - (ypos / height) * 2.0f;

        activeWhiteboard->vertices.push_back(x);
        activeWhiteboard->vertices.push_back(y);

        activeWhiteboard->curr_xpos = xpos;
        activeWhiteboard->curr_ypos = ypos;
    }
}

void Whiteboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (activeWhiteboard && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (key == GLFW_KEY_RIGHT_BRACKET) {
            activeWhiteboard->brushSize = std::min(10.0f, activeWhiteboard->brushSize + 1.0f);
        }
        else if (key == GLFW_KEY_LEFT_BRACKET) {
            activeWhiteboard->brushSize = std::max(1.0f, activeWhiteboard->brushSize - 1.0f);
        }
        else if (key == GLFW_KEY_UP) {
            activeWhiteboard->currentColor[3] = std::min(1.0f, activeWhiteboard->currentColor[3] + 0.1f);
        }
        else if (key == GLFW_KEY_DOWN) {
            activeWhiteboard->currentColor[3] = std::max(0.1f, activeWhiteboard->currentColor[3] - 0.1f);
        }
        else if (key == GLFW_KEY_R) {
            activeWhiteboard->resetBrushSize();
        }
        else if (key == GLFW_KEY_1) {
            activeWhiteboard->setBrushColor(0.0f, 0.0f, 0.0f, 1.0f);
        }
        else if (key == GLFW_KEY_2) {
            activeWhiteboard->setBrushColor(1.0f, 0.0f, 0.0f, 1.0f);
        }
        else if (key == GLFW_KEY_3) {
            activeWhiteboard->setBrushColor(0.0f, 1.0f, 0.0f, 1.0f);
        }
        else if (key == GLFW_KEY_4) {
            activeWhiteboard->setBrushColor(0.0f, 0.0f, 1.0f, 1.0f);
        }
        else if (key == GLFW_KEY_Q) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
}

void Whiteboard::draw() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //std::cout << "drawing!" << std::endl;
    for (auto& action : drawActions) {
        action.draw();
    }

    glPointSize(brushSize);
    glLineWidth(brushSize);

    glEnableClientState(GL_VERTEX_ARRAY);
    if (vertices.size() >= 4) {
        glVertexPointer(2, GL_FLOAT, 0, vertices.data());
        glColor4fv(currentColor);
        glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 2);
    }
    else if (vertices.size() == 2) {
        glVertexPointer(2, GL_FLOAT, 0, vertices.data());
        glColor4fv(currentColor);
        glDrawArrays(GL_POINTS, 0, 1);
    }
    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}

Whiteboard::Whiteboard(std::string user) {
    this->myId = user;
    activeWhiteboard = this;
}

Whiteboard::~Whiteboard() {
    activeWhiteboard = nullptr;
}

void Whiteboard::setWindow(GLFWwindow* newWindow) {
    window = newWindow;
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetKeyCallback(window, keyCallback);
}

void Whiteboard::render() {
    draw();
}

void Whiteboard::resetBrushSize() {
    brushSize = 2.0f;
}

void Whiteboard::setBrushSize(float size) {
    brushSize = size;
}

void Whiteboard::setBrushColor(float r, float g, float b, float a) {
    currentColor[0] = r;
    currentColor[1] = g;
    currentColor[2] = b;
    currentColor[3] = a;
}

bool Whiteboard::addDrawAction(std::string user, float brushSize, float r, float g, float b, float a, std::vector<float> vertices) {
    if (vertices.size() % 2 != 0) {
        std::cerr << "Error: Vertices must contain pairs of x and y coordinates." << std::endl;
        return false;
    }

    if (r < 0.0f || r > 1.0f || g < 0.0f || g > 1.0f || b < 0.0f || b > 1.0f || a < 0.0f || a > 1.0f) {
        std::cerr << "Error: Color components must be in the range [0, 1]." << std::endl;
        return false;
    }

    float color[4] = { r, g, b, a };

    DrawAction newAction(user, vertices, brushSize, color);
    prevActions.push_back(newAction);
    drawActions.push_back(newAction);

    return true;
}

bool Whiteboard::addDrawAction(DrawAction dAction) {
    if (dAction.vertices.size() % 2 != 0) {
        std::cerr << "Error: Vertices must contain pairs of x and y coordinates." << std::endl;
        return false;
    }
    if (dAction.color[0] < 0.0f || dAction.color[0] > 1.0f || dAction.color[1] < 0.0f || dAction.color[1] > 1.0f || dAction.color[2] < 0.0f || dAction.color[2] > 1.0f || dAction.color[3] < 0.0f || dAction.color[3] > 1.0f) {
        std::cerr << "Error: Color components must be in the range [0, 1]." << std::endl;
        return false;
    }

    prevActions.push_back(dAction);
    drawActions.push_back(dAction);
    return true;
}

Whiteboard* getActiveWhiteboard(){
    return activeWhiteboard;
}

#endif