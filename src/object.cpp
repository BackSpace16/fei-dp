#include <main.hpp>
#include "shapes.cpp"
#include <iostream>

class Camera {
    private:
        Settings& settings;
    public:
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        Camera(Settings& settings) : settings{settings} {
            viewMatrix = glm::mat4(1.0f);
            glm::vec3 position{0.0f, 0.0f, 5.0f};
            glm::vec3 target{0.0f, 0.0f, 0.0f};
            glm::vec3 up{0.0f, 1.0f, 0.0f};

            viewMatrix = glm::lookAt(
                position,        // pozícia kamery (pohľad z osi Z smerom k scéne)
                target,         // bod, na ktorý kamera pozerá
                up             // smer nahor
            );

            float aspectRatio = static_cast<float>(settings.w_width) / static_cast<float>(settings.w_height);
            projectionMatrix = glm::mat4(1.0f);
            projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
            //projectionMatrix = glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, 0.1f, 100.0f);
            
        }

        void set(Shader& shader) {
            shader.setUniformMat4("view", viewMatrix);
            shader.setUniformMat4("projection", projectionMatrix);
        }
        
    // TODO camera movement, camera light
};

template<typename MeshType>
class Object {
    public:
        MeshType& mesh;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 color;
        glm::mat4 modelMatrix;
        bool move;

        Object(MeshType& mesh,
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
            rotation = glm::vec3(glm::degrees(degrees), glm::degrees(degrees), 0.0f);
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
