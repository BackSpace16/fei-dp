#include <main.hpp>
#include "shapes.cpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint WIDTH = 800, HEIGHT = 600;

void GLAPIENTRY errorOccurredGL(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam) {
    printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
            "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
    printf("%s\n", message);
    exit(-1); // shut down the program gracefully (it does cleanup stuff too)
    // without exit(), OpenGL will constantly print the error message... make sure to kill your program.
}

int main(void) {
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error: " << std::endl;
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    //**********************************************//

    std::cout << glGetString(GL_VERSION) << std::endl;

    ShaderProgramSource source = parseShader("../../res/color.shader");
    GLuint shader = createShader(source.vertexSource, source.fragmentSource);

    ShaderProgramSource lampSource = parseShader("../../res/lamp.shader");
    GLuint lampShader = createShader(lampSource.vertexSource, lampSource.fragmentSource);

    ShaderProgramSource shadedSource = parseShader("../../res/shaded.shader");
    GLuint shadedShader = createShader(shadedSource.vertexSource, shadedSource.fragmentSource);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorOccurredGL, NULL);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    Cube d(1.0f);
    Icosphere c(0.8f, 2);
    Cube e(0.4f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(1, 1, 1));
        
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 3.0f), // pozícia kamery (pohľad z osi Z smerom k scéne)
            glm::vec3(0.0f, 0.0f, 0.0f), // bod, na ktorý kamera pozerá
            glm::vec3(0.0f, 1.0f, 0.0f)  // smer nahor
        );

        float aspectRatio = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
        glm::mat4 projection = glm::mat4(1.0f);
        //projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        projection = glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, 0.1f, 100.0f);
        
        glUseProgram(shadedShader);
        // Get matrix's uniform location and set matrix
        GLint modelLoc = glGetUniformLocation(shadedShader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        GLint viewLoc = glGetUniformLocation(shadedShader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        GLint projectionLoc = glGetUniformLocation(shadedShader, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        GLint lightPosLoc = glGetUniformLocation(shadedShader, "lightPos");
        GLint lightColorLoc = glGetUniformLocation(shadedShader, "lightColor");
        GLint objectColorLoc = glGetUniformLocation(shadedShader, "objectColor");

        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
        glUniform3f(objectColorLoc, 1.0f, 0.0f, 0.0f);

        c.draw();


        glUseProgram(shader);
        // Get matrix's uniform location and set matrix
        modelLoc = glGetUniformLocation(shader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        viewLoc = glGetUniformLocation(shader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        projectionLoc = glGetUniformLocation(shader, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        GLint colorLoc = glGetUniformLocation(shader, "out_color");
        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 0.0f);

        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        
        model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 0.0f);
        //d.draw();

        glUseProgram(lampShader);
        modelLoc = glGetUniformLocation(lampShader, "model");
        viewLoc = glGetUniformLocation(lampShader, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        projectionLoc = glGetUniformLocation(lampShader, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 0.0f);
        e.draw();

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        glfwSwapBuffers(window);
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
