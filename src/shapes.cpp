#pragma once
#include <main.hpp>

#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <array>
#include <span>
#include <optional>

#include <iostream>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;

    Vertex(const glm::vec3& pos, const glm::vec3& norm)
        : position{pos}, normal{norm} {}

    bool operator==(const Vertex& other) const {
        return position == other.position && normal == other.normal;
    }
};

struct Triangle {
    std::array<Vertex, 3> vertices;

    Triangle(const std::array<glm::vec3, 3>& positions, const glm::vec3& normal)
        : vertices{ Vertex(positions[0], normal), Vertex(positions[1], normal), Vertex(positions[2], normal) } {}
};

class Mesh {
    public:
        std::vector<Triangle> triangles;

        GLuint VAO;
        GLuint VBO;

        void draw() {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
            glBindVertexArray(0);
        }

        void loadBufferData() {
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

    protected:
        void addTriangle(const std::array<glm::vec3,3>& positions) {
            glm::vec3 normal = glm::normalize(glm::cross(positions[1] - positions[0], positions[2] - positions[0]));
            triangles.emplace_back(positions, normal);
        }

        void addTriangle(std::vector<Triangle>& triangleList, const std::array<glm::vec3,3>& positions) {
            glm::vec3 normal = glm::normalize(glm::cross(positions[1] - positions[0], positions[2] - positions[0]));
            triangleList.emplace_back(positions, normal);
        }

        void addGeometry(const std::span<glm::vec3>& vertices, const std::span<std::array<unsigned int, 3>>& indices) {
            for (const auto& triangle : indices)
                addTriangle({ vertices[triangle[0]], vertices[triangle[1]], vertices[triangle[2]] });
        }

        float calcSize(Vertex vertex, Vertex vertex2) {
            return sqrt(pow((vertex.position.x - vertex2.position.x),2)
                   +pow((vertex.position.y - vertex2.position.y),2)
                   +pow((vertex.position.z - vertex2.position.z),2));
        }

        void calcTriangles() {
            for(Triangle& triangle : triangles) {
                float a = calcSize(triangle.vertices[0], triangle.vertices[1]);
                float b = calcSize(triangle.vertices[1], triangle.vertices[2]);
                float c = calcSize(triangle.vertices[0], triangle.vertices[2]);
                std::cout << a << ", " << b << ", " << c << ", " << std::endl;
            }
        }
};

class Cube : public Mesh {
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
                {1, 2, 0},
                {2, 1, 3},
                {6, 5, 4},
                {5, 6, 7},
                {2, 7, 6},
                {7, 2, 3},
                {5, 0, 4},
                {0, 5, 1},
                {7, 1, 5},
                {1, 7, 3},
                {0, 6, 4},
                {6, 0, 2}
            }};

            addGeometry(vertices, indices);
        }
};

class Icosahedron : public Mesh {
    private:
        const float H_ANGLE = M_PI / 180 * 72;    // 72 degree = 360 / 5
        const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    public:
        Icosahedron(float radius) {
            createGeometry(radius);
            
            //calcTriangles();
        }

    private:
        void createGeometry(float radius) {
            float z, xy;
            float uAngle = -M_PI / 2 - H_ANGLE / 2;  // -126 deg (234 deg = 90 + 2*72 )
            float lAngle = -M_PI / 2;               // -90 deg  (270 deg)

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
                addTriangle({ upperFirstPos, lowerFirstPos, upperSecondPos });
                addTriangle({ upperSecondPos, lowerFirstPos, lowerSecondPos });
                addTriangle({ lowerFirstPos, southPos, lowerSecondPos });

                uAngle += H_ANGLE;
                lAngle += H_ANGLE;
            }
        }
};

class Icosphere : public Icosahedron {
    private:
        glm::vec3 updateRadius(const glm::vec3& position, const float radius) {
            float r = glm::length(position);              // radius
            float phi = atan2(position.y, position.x);   // azimuthal angle
            float theta  = acos(position.z / r);        // polar angle

            float x = radius * sin(theta) * cos(phi);
            float y = radius * sin(theta) * sin(phi);
            float z = radius * cos(theta);

            return glm::vec3{x,y,z};
        }

    public:
        Icosphere(float radius, unsigned int subdivision) : Icosahedron(radius) {
            subdivideTriangles(radius, subdivision);
            //calcTriangles();
        }

    private:
        void subdivideTriangles(float radius, unsigned int subdivision) {
            std::vector<Triangle> new_triangles;
            for (Triangle& triangle : triangles) {

                glm::vec3 c1 = (triangle.vertices[0].position + triangle.vertices[1].position) / 2.0f;
                glm::vec3 c2 = (triangle.vertices[0].position + triangle.vertices[2].position) / 2.0f;
                glm::vec3 c3 = (triangle.vertices[1].position + triangle.vertices[2].position) / 2.0f;

                c1 = updateRadius(c1, radius);
                c2 = updateRadius(c2, radius);
                c3 = updateRadius(c3, radius);

                glm::vec3 e2 = triangle.vertices[1].position;
                glm::vec3 e3 = triangle.vertices[2].position;

                glm::vec3 normal = glm::normalize(glm::cross(c1 - triangle.vertices[0].position, c2 - triangle.vertices[0].position));

                triangle.vertices[0].normal = normal;
                triangle.vertices[1] = Vertex(c1, normal);
                triangle.vertices[2] = Vertex(c2, normal);

                addTriangle(new_triangles, {c1,c3,c2});
                addTriangle(new_triangles, {c1,e2,c3});
                addTriangle(new_triangles, {c2,c3,e3});
                
            }
            triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
        }
};
