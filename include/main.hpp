#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nlohmann/json.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <unordered_set>
#include <unordered_map>
#include <variant>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <array>
#include <span>
#include <optional>
#include <memory>

extern void GLAPIENTRY errorOccurredGL(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*);
extern void framebuffer_size_callback(GLFWwindow*, int, int);

struct Color {
    float r;
    float g;
    float b;
    float a;

    Color(float red, float green, float blue, float alpha = 1.0f);
    Color(int red, int green, int blue, int alpha = 255);
    Color(const std::string& hex);

    glm::vec3 toVec3() const;
    glm::vec4 toVec4() const;

    void printRGB() const;
    void printHex() const;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;

    Vertex(const glm::vec3& pos, const glm::vec3& norm);
    bool operator==(const Vertex& other) const;
};

struct Triangle {
    std::array<size_t, 3> vertices;
    std::array<size_t, 3> normals;

    Triangle();
    Triangle(const std::array<size_t, 3>& positions, const std::array<size_t, 3>& normals);
};

class MeshType {
    public:
        virtual void loadBufferData() = 0;
        virtual void draw() = 0;
        virtual void smoothSurface() = 0;
        virtual ~MeshType() = default;
        virtual void loadBufferData(const std::vector<glm::mat4>& modelMatrices, const std::vector<glm::vec3>& colors) = 0;
        virtual void drawInstances(size_t instanceCount) = 0;
};

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>  // Normals per vertex
class Mesh : public MeshType {
    public:
        std::array<Triangle, MAX_TRIANGLES> triangles;
        size_t triangleCount = 0;

        std::array<glm::vec3, MAX_VERTICES> vertexPositions;
        size_t vertexCount = 0;

        std::array<std::array<glm::vec3, MAX_NORMALS>, MAX_VERTICES> normals = {};

        GLuint VAO;
        GLuint VBO;

        GLuint modelMatrixVBO;
        GLuint colorVBO;

        size_t getMaxVertices();
        size_t getMaxTriangles();
        size_t getMaxNormals();
        
        void draw();
        void loadBufferData();

        void drawInstances(size_t instanceCount);
        void loadBufferData(const std::vector<glm::mat4>& modelMatrices, const std::vector<glm::vec3>& colors);

        void smoothSurface();
    protected:
        bool comparePositions(const glm::vec3& position1, const glm::vec3& position2);
        size_t findVertex(const glm::vec3& position);
        size_t addVertex(const glm::vec3& position);
        size_t findOrAddVertex(const glm::vec3& position);
        size_t addNormal(const size_t index, const glm::vec3& newNormal);
        void addTriangle(const std::array<glm::vec3,3>& positions);
        void addTriangle(const std::array<size_t,3>& positionIndices);
        void addGeometry(const std::span<glm::vec3>& vertices, const std::span<std::array<unsigned int, 3>>& indices);
};

class Cube : public Mesh<12,8,3> {
    public:
        Cube(float size);
};

class Icosahedron : public Mesh<20,12,5> {
    private:
        const float H_ANGLE = M_PI / 180 * 72;    // 72 degree = 360 / 5
        const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree
        void createGeometry(float radius);
    public:
        Icosahedron(float radius);
};

constexpr size_t calculateMaxTriangles(size_t N) {
    return 20 * N * N;
}
constexpr size_t calculateMaxVertices(size_t N) {
    return 12 + (30 * (N-1));
}

class Icosphere : public Mesh<500,500,50> {// Mesh<calculateMaxTriangles(N_SUBDIVISION), calculateMaxVertices(N_SUBDIVISION), 10>
    public:
        Icosphere(size_t subdivision, float radius);
    private:
        const float H_ANGLE = M_PI / 180 * 72;    // 72 degree = 360 / 5
        const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree
        const size_t N_SUBDIVISION;
        glm::vec3 updateRadius(const glm::vec3& position, const float radius);
        void subdivideTriangles(float radius);
        void createGeometry(float radius);
};

class Settings {
    private:
        void loadDefault();
    public:
        unsigned int w_width;
        unsigned int w_height;
        std::string w_name;
        Color backgroundColor{"#000"};

        Settings();
};

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

ShaderProgramSource parseShader(const std::string& filePath);
GLuint compileShader(GLuint type, const std::string& source);
GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader);

class Shader {
public:
    using UniformValue = std::variant<int, float, glm::vec3, glm::mat4>; 

    std::unordered_map<std::string, GLint> uniforms;
    GLuint shader;

    Shader(const std::string path, const std::initializer_list<std::string>& uniformsList);
    ~Shader();

    void setUniformMat4(const std::string& uniform, const glm::mat4& matrix);
    void setUniformVec4(const std::string& uniform, const glm::vec4 vector);
    void setUniformVec3(const std::string& uniform, const glm::vec3 vector);
    void use();
};

class Camera {
    private:
        Settings& settings;
    public:
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;

        Camera(Settings& settings);
        void set(Shader& shader);
        
    // TODO camera movement, camera light
};

class Object {
    public:
        std::shared_ptr<MeshType> mesh;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 color;
        glm::mat4 modelMatrix;
        bool move;
        
        Object(std::shared_ptr<MeshType> mesh,
                       glm::vec3 position = glm::vec3(0.0f), 
                       glm::vec3 rotation = glm::vec3(0.0f), 
                       glm::vec3 scale = glm::vec3(1.0f),
                       glm::vec3 color = glm::vec3(1.0f));

        void changeRotation(float degrees);
        glm::mat4 getModelMatrix();
        glm::vec3 getColor();

    private:
        void transform();
};

class Scene {
    private:
        Settings& settings;
    public:
        std::unordered_set<std::shared_ptr<MeshType>> meshes;
        std::vector<std::unique_ptr<Object>> objects;

        Scene(Settings& settings);
        void init();
        void draw(Shader& shader);
        void addMesh(std::shared_ptr<MeshType> mesh);
        void addObject(std::unique_ptr<Object> object);
};

class App {
    private:
        Settings settings;
        GLFWwindow* glfwWindow;
        Scene scene{settings};
    public:
        App();
        ~App();
        GLFWwindow* initOpenGL();
};
