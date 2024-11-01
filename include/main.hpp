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

#include <unordered_map>
#include <variant>

extern void GLAPIENTRY errorOccurredGL(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*);
extern void framebuffer_size_callback(GLFWwindow*, int, int);

class Settings {
    private:
        void loadDefault();
    public:
        unsigned int w_width;
        unsigned int w_height;
        std::string w_name;

        Settings();
};

class App {
    private:
        Settings settings;
        GLFWwindow* glfwWindow;
    public:
        App();
        //~App();
        GLFWwindow* initOpenGL();
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
