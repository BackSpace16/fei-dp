#include <main.hpp>

/*class Data {
    private:
    public:
        Data();
        ~Data();
};*/

/*Scene::Scene() {

}*/

GLFWwindow* App::initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Error: glfw init" << std::endl;
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(settings.w_width, settings.w_height, settings.w_name.c_str(), NULL, NULL);
    if (!window) {
        std::cerr << "Error: glfw create window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error: glew init " << std::endl;
        exit(-1);
    }

    glViewport(0, 0, settings.w_width, settings.w_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorOccurredGL, NULL);

    return window;
}

App::App() {
    glfwWindow = initOpenGL();
    std::cout << glGetString(GL_VERSION) << std::endl;

    Shader colorShader{"../res/shaders/color.shader", {"model", "view", "projection", "out_color"}};
    Shader lampShader{"../res/shaders/lamp.shader", {"model", "view", "projection"}};
    Shader shadedShader{"../res/shaders/shaded.shader", {"model", "view", "projection", "lightPos", "lightColor", "objectColor"}};

    glm::vec3 lightPos(0.0f, 0.0f, 5.0f);
    
    Icosphere cubex(5, 0.8f);
    Object p99(cubex,
            glm::vec3(1.0f, 0.8f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        
    cubex.loadBufferData();

    Camera c{settings};
    c.set(shadedShader);
    
    shadedShader.setUniformVec3("lightPos", lightPos);
    shadedShader.setUniformVec3("lightColor", Color{"#fff"}.toVec3());// glm::vec3(1.0f, 1.0f, 1.0f)
    
    glClearColor(settings.backgroundColor.r, settings.backgroundColor.g, settings.backgroundColor.b, settings.backgroundColor.a);

    while (!glfwWindowShouldClose(glfwWindow)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        p99.changeRotation((float)glfwGetTime());
        p99.draw(shadedShader);

        glfwSwapBuffers(glfwWindow);
    }
}

App::~App() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

Settings::Settings() {
    loadDefault();
}

void Settings::loadDefault() {
    std::ifstream file("../res/settings.cfg");
    if (file.is_open()) {
        nlohmann::json jsonConfig;
        file >> jsonConfig;
        w_name = jsonConfig.value("w_name", "okno");
        w_width = jsonConfig.value("w_width", 800);
        w_height = jsonConfig.value("w_height", 600);
        backgroundColor = Color{jsonConfig.value("backgroundColor", "#000")};
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
