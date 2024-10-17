#include <main.hpp>
#include "shapes.cpp"
#include <iostream>

class Object {
    public:
        Mesh mesh;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 color;
        glm::mat4 modelMatrix;
        bool move;

        Object(Mesh mesh,
               glm::vec3 position = glm::vec3(0.0f), 
               glm::vec3 rotation = glm::vec3(0.0f), 
               glm::vec3 scale = glm::vec3(1.0f),
               glm::vec3 color = glm::vec3(1.0f)) 
            : mesh(mesh), position(position), rotation(rotation), scale(scale), color(color) {
            mesh.loadBufferData();
            transform();
            move = false;
        }

        void draw(Shader& shader) {
            shader.setUniformMat4("model", modelMatrix);
            shader.setUniformVec3("objectColor", color);
            mesh.draw();
        }

        void changeRotation(float degrees) {
            rotation = glm::vec3(0.0f, glm::degrees(degrees), 0.0f);
            transform();
        }

        glm::mat4 getModelMatrix() {
            return modelMatrix;
        }

    private:
        void transform() {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMatrix = glm::scale(modelMatrix, scale);
        }
};
