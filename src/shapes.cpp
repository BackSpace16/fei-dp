#include <main.hpp>

#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <array>
#include <span>
#include <optional>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;

    Vertex(const glm::vec3& pos, const glm::vec3& norm)
        : position{pos}, normal{norm} {}
};

struct Triangle {
    std::array<Vertex, 3> vertices;

    Triangle(const std::array<glm::vec3, 3>& positions, const glm::vec3& normal)
        : vertices{ Vertex(positions[0], normal), Vertex(positions[1], normal), Vertex(positions[2], normal) } {}
};

class Shape {
    public:
        std::vector<Triangle> triangles;

        GLuint VAO;
        GLuint VBO;

        void createShape() {
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            std::vector<Vertex> vertexData;
            for (const Triangle& triangle : triangles) {
                for (const Vertex& vertex : triangle.vertices) {
                    vertexData.push_back(vertex);
                }
            }
            glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }

        void draw() {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
            glBindVertexArray(0);
        }

        void addTriangle(const std::array<glm::vec3,3>& positions) {
            glm::vec3 normal = glm::normalize(glm::cross(positions[1] - positions[0], positions[2] - positions[0]));
            triangles.emplace_back(positions, normal);
        }

        void addGeometry(const std::span<glm::vec3>& vertices, const std::span<std::array<unsigned int, 3>>& indices) {
            for (const auto& triangle : indices)
                addTriangle({ vertices[triangle[0]], vertices[triangle[1]], vertices[triangle[2]] });
        }
};

class Cube : public Shape {
    public:
        Cube(float size) {
            float halfsize = size / 2;

            std::array<glm::vec3,8> vertices{ 
                glm::vec3(-halfsize, -halfsize, -halfsize),
                glm::vec3(-halfsize,  halfsize, -halfsize),
                glm::vec3( halfsize, -halfsize, -halfsize),
                glm::vec3( halfsize,  halfsize, -halfsize),

                glm::vec3(-halfsize, -halfsize,  halfsize),
                glm::vec3(-halfsize,  halfsize,  halfsize),
                glm::vec3( halfsize, -halfsize,  halfsize),
                glm::vec3( halfsize,  halfsize,  halfsize)
            };

            std::array<std::array<unsigned int,3>,12> indices{{
                {2, 1, 0},
                {1, 2, 3},
                {6, 5, 4},
                {5, 6, 7},
                {2, 7, 6},
                {7, 2, 3},
                {0, 5, 4},
                {5, 0, 1},
                {7, 1, 5},
                {1, 7, 3},
                {6, 0, 4},
                {0, 6, 2}
            }};

            addGeometry(vertices, indices);
            createShape();
        }
};

class Icosahedron : public Shape {
    private:
        const float H_ANGLE = M_PI / 180 * 72;    // 72 degree = 360 / 5
        const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    public:
        Icosahedron(float radius) {
            float z, xy;
            float uAngle = -M_PI / 2 - H_ANGLE / 2;
            float lAngle = -M_PI / 2;

            glm::vec3 northPos{0,0,radius};
            glm::vec3 southPos{0,0,-radius};

            for(int i = 1; i <= 5; ++i) {
                z  = radius * sinf(V_ANGLE);
                xy = radius * cosf(V_ANGLE);

                glm::vec3 upperFirstPos{xy * cosf(uAngle), xy * sinf(uAngle), z};
                glm::vec3 upperSecondPos{xy * cosf(uAngle+H_ANGLE), xy * sinf(uAngle+H_ANGLE), z};

                
                glm::vec3 lowerFirstPos{xy * cosf(lAngle), xy * sinf(lAngle), -z};
                glm::vec3 lowerSecondPos{xy * cosf(lAngle+H_ANGLE), xy * sinf(lAngle+H_ANGLE), -z};

                addTriangle({ northPos, upperFirstPos, upperSecondPos });
                addTriangle({ upperFirstPos, upperSecondPos, lowerFirstPos });
                addTriangle({ upperSecondPos, lowerFirstPos, lowerSecondPos });
                addTriangle({ lowerFirstPos, southPos, lowerSecondPos });

                uAngle += H_ANGLE;
                lAngle += H_ANGLE;
            }

            createShape();
        }
};
