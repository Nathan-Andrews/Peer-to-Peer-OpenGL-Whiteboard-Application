#ifndef DRAW_ACTION_H
#define DRAW_ACTION_H

#define GLEW_STATIC

#include "Source.h"

struct DrawAction {
public:
    std::string id;
    std::vector<float> vertices;
    float brushSize;
    float color[4]; // RGBA

    DrawAction(const std::string& identifier, const std::vector<float>& vert, float size, const float* c)
        : id(identifier), vertices(vert), brushSize(size) {
        color[0] = c[0]; color[1] = c[1]; color[2] = c[2]; color[3] = c[3];
    }
    DrawAction(){}

    std::string serialize() const {
        std::string serialized;
        
        uint32_t idLength = static_cast<uint32_t>(id.length());
        uint32_t vertSize = static_cast<uint32_t>(vertices.size());
        
        serialized.reserve(sizeof(uint32_t) + id.length() + 
                        sizeof(uint32_t) + vertices.size() * sizeof(float) + 
                        sizeof(float) + 4 * sizeof(float));
        
        serialized.append(reinterpret_cast<const char*>(&idLength), sizeof(uint32_t));
        serialized.append(id);
        
        serialized.append(reinterpret_cast<const char*>(&vertSize), sizeof(uint32_t));
        serialized.append(reinterpret_cast<const char*>(vertices.data()), vertices.size() * sizeof(float));
        serialized.append(reinterpret_cast<const char*>(&brushSize), sizeof(float));
        serialized.append(reinterpret_cast<const char*>(color), 4 * sizeof(float));
        
        return serialized;
    }

    static DrawAction deserialize(const std::string& data) {
        DrawAction action;
        size_t offset = 0;

        if (data.size() < sizeof(uint32_t)) {
            throw std::runtime_error("Data too small for id length");
        }

        uint32_t idLength;
        std::memcpy(&idLength, data.data() + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        if (data.size() < offset + idLength) {
            throw std::runtime_error("Data too small for id string");
        }
        action.id = data.substr(offset, idLength);
        offset += idLength;

        if (data.size() < offset + sizeof(uint32_t)) {
            throw std::runtime_error("Data too small for vertices size");
        }
        uint32_t vertSize;
        std::memcpy(&vertSize, data.data() + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        if (data.size() < offset + (vertSize * sizeof(float))) {
            throw std::runtime_error("Data too small for vertices");
        }
        action.vertices.resize(vertSize);
        std::memcpy(action.vertices.data(), data.data() + offset, vertSize * sizeof(float));
        offset += vertSize * sizeof(float);

        if (data.size() < offset + sizeof(float) + (4 * sizeof(float))) {
            throw std::runtime_error("Data too small for brush size + color");
        }
        std::memcpy(&action.brushSize, data.data() + offset, sizeof(float));
        offset += sizeof(float);
        std::memcpy(action.color, data.data() + offset, 4 * sizeof(float));

        return action;
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
