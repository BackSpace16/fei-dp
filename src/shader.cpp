#include <main.hpp>
#include <unordered_map>
#include <variant>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

ShaderProgramSource parseShader(const std::string& filePath) {
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::ifstream stream(filePath);

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;

            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(), ss[1].str()};
}

GLuint compileShader(GLuint type, const std::string& source) {
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

Shader::Shader(const std::string path, const std::initializer_list<std::string>& uniformsList) {
    ShaderProgramSource source = parseShader(path);
    shader = createShader(source.vertexSource, source.fragmentSource);

    glUseProgram(shader);
    for (const std::string& uniform : uniformsList)
        uniforms[uniform] = glGetUniformLocation(shader, uniform.c_str());

    
}

Shader::~Shader() {
    glDeleteProgram(shader);
}

void Shader::setUniformMat4(const std::string& uniform, const glm::mat4& matrix) {
    glUseProgram(shader);
    glUniformMatrix4fv(uniforms[uniform], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformVec4(const std::string& uniform, const glm::vec4 vector) {
    glUseProgram(shader);
    glUniform4fv(uniforms[uniform], 1, glm::value_ptr(vector));

}

void Shader::setUniformVec3(const std::string& uniform, const glm::vec3 vector) {
    glUseProgram(shader);
    glUniform3fv(uniforms[uniform], 1, glm::value_ptr(vector));
}

void Shader::use() {
    glUseProgram(shader);
}