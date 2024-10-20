#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <unordered_map>
#include <variant>

const GLuint WIDTH = 1800, HEIGHT = 1200;

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
