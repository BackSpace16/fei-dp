#include <main.hpp>
#include "shapes.cpp"
#include "object.cpp"

/*class Data {
    private:
    public:
        Data();
        ~Data();
};


class Scene {
    private:
    public:
        Scene();
        ~Scene();
};*/

App::App() {
    if (!glfwInit()) {
        std::cout << "Error glfw init" << std::endl;
        exit(-1);
        //return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(settings.w_width, settings.w_height, "Hello World", NULL, NULL);
    if (!window) {
        std::cout << "Error glfw create window" << std::endl;
        glfwTerminate();
        exit(-1);
        //return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error: " << std::endl;        

    glViewport(0, 0, settings.w_width, settings.w_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    //**********************************************//

    std::cout << glGetString(GL_VERSION) << std::endl;

    Shader colorShader{"../res/shaders/color.shader", {"model", "view", "projection", "out_color"}};
    Shader lampShader{"../res/shaders/lamp.shader", {"model", "view", "projection"}};
    Shader shadedShader{"../res/shaders/shaded.shader", {"model", "view", "projection", "lightPos", "lightColor", "objectColor"}};

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorOccurredGL, NULL);

    glm::vec3 lightPos(0.0f, 0.0f, 5.0f);

    
    Icosphere<4> cubex(0.8f);
    Object<Icosphere<4>> p99(cubex,
            glm::vec3(1.0f, 0.8f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    cubex.loadBufferData();

    
    Camera c{settings};
    c.set(shadedShader);
    
    shadedShader.setUniformVec3("lightPos", lightPos);
    shadedShader.setUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        p99.changeRotation((float)glfwGetTime());
        p99.draw(shadedShader);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

/*App::~App() {
}

*/

Settings::Settings() {
    loadDefault();
}

void Settings::loadDefault() {
    std::ifstream file("../res/settings.cfg");
    if (file.is_open()) {
        nlohmann::json jsonConfig;
        file >> jsonConfig;
        w_width = jsonConfig.value("w_width", 800);
        w_height = jsonConfig.value("w_height", 600);
        std::cout << w_width << std::endl;
    } 
    else {
        std::cerr << "Failed to open settings.cfg" << std::endl;
    }
}

/*

Data::Data() {
}

Data::~Data() {
}

Scene::Scene() {
}

Scene::~Scene() {
}*/