#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

ShaderProgramSource parseShader(const std::string& filePath);
GLuint compileShader(GLuint type, const std::string& source);
GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader);
