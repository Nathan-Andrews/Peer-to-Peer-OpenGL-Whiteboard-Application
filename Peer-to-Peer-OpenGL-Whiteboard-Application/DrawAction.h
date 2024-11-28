#ifndef DRAW_ACTION_H
#define DRAW_ACTION_H

#define GLEW_STATIC

#include <string>
#include <vector>

#include <GL/glew.h>

class DrawAction {
public:
    std::string id;
    std::vector<float> vertices;
    float brushSize;
    float color[4]; // RGBA

    DrawAction(const std::string& identifier, const std::vector<float>& vert, float size, const float* c)
        : id(identifier), vertices(vert), brushSize(size) {
        color[0] = c[0]; color[1] = c[1]; color[2] = c[2]; color[3] = c[3];
    }

    void draw() const {
        glPointSize(brushSize);
        glLineWidth(brushSize);
        glEnableClientState(GL_VERTEX_ARRAY);
        if (vertices.size() == 2) {
            glVertexPointer(2, GL_FLOAT, 0, vertices.data());
            glColor4fv(color);
            glDrawArrays(GL_POINTS, 0, 1);
        }
        else {
            glLineWidth(brushSize);
            glVertexPointer(2, GL_FLOAT, 0, vertices.data());
            glColor4fv(color);
            glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 2);
        }
        glDisableClientState(GL_VERTEX_ARRAY);
    }
};

#endif
