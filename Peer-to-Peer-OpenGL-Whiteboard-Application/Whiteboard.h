#pragma once
#ifndef WHITEBOARD_H
#define WHITEBOARD_H

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

#include "DrawAction.h"
#include "ConnectionManager.h"

class Whiteboard {
private:
    std::vector<float> vertices;
    std::vector<DrawAction> drawActions;
    bool mouseDownOnWindow = false;
    float brushSize = 2.0f;

    double curr_xpos = 0.0f;
    double curr_ypos = 0.0f;

    std::string myId;
    float currentColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    GLFWwindow* window;

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void draw();

public:
    Whiteboard(std::string user);
    ~Whiteboard();

    void setWindow(GLFWwindow* newWindow);
    void render();
    void resetBrushSize();
    void setBrushSize(float size);
    void setBrushColor(float r, float g, float b, float a);
    bool addDrawAction(std::string user, float brushSize, float r, float g, float b, float a, std::vector<float> vertices);
    bool addDrawAction(DrawAction dAction);
    void setConnectionManager(ConnectionManager* manager);
};

#endif
