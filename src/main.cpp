#include <main.hpp>
#include "shapes.cpp"
#include "object.cpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

        

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    //**********************************************//

    std::cout << glGetString(GL_VERSION) << std::endl;

    Shader colorShader{"../../res/color.shader", {"model", "view", "projection", "out_color"}};
    Shader lampShader{"../../res/lamp.shader", {"model", "view", "projection"}};
    Shader shadedShader{"../../res/shaded.shader", {"model", "view", "projection", "lightPos", "lightColor", "objectColor"}};

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorOccurredGL, NULL);

    glm::vec3 lightPos(0.0f, 0.0f, 5.0f);

    /*Object b(Icosahedron(1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    Object a(Icosphere(0.8f, 2),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 1.0f));*/

        
    Icosphere<2> cube(0.8f);
    Object<Icosphere<2>> p(cube,
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f));


    Icosahedron cubeb(0.8f);
    Object<Icosahedron> d(cubeb,
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
            
    //d.mesh.smoothSurface();
    d.mesh.loadBufferData();

    //p.mesh.smoothSurface();
    p.mesh.loadBufferData();

    //b.mesh.loadBufferData();
    //a.mesh.loadBufferData();

    Camera c{};
    c.set(shadedShader);
    
    shadedShader.setUniformVec3("lightPos", lightPos);
    shadedShader.setUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    
    /********************* lampáš **********************
    Cube e(0.2f);
    e.loadBufferData();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    lampShader.setUniformMat4("model", model);
    lampShader.setUniformMat4("view", view);
    lampShader.setUniformMat4("projection", projection);
    */

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //a.changeRotation(sin((float)glfwGetTime()));
        //a.draw(shadedShader);
        
        //b.changeRotation(-(float)glfwGetTime());
        //b.draw(shadedShader);

        d.changeRotation(-(float)glfwGetTime());
        d.draw(shadedShader);

        p.changeRotation(sin((float)glfwGetTime()));
        p.draw(shadedShader);

        //e.draw();
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
