#include <main.hpp>

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
    Shader shadedInstanceShader{"../res/shaders/shadedinstance.shader", {"view", "projection", "lightPos", "lightColor"}};

    glm::vec3 lightPos(0.0f, 0.0f, 35.0f);
    
    Camera camera{settings};
    camera.set(shadedShader);
    camera.set(shadedInstanceShader);
    
    shadedShader.setUniformVec3("lightPos", lightPos);
    shadedShader.setUniformVec3("lightColor", Color{"#fff"}.toVec3());// glm::vec3(1.0f, 1.0f, 1.0f)

    shadedInstanceShader.setUniformVec3("lightPos", lightPos);
    shadedInstanceShader.setUniformVec3("lightColor", Color{"#fff"}.toVec3());// glm::vec3(1.0f, 1.0f, 1.0f)

    //Icosphere cubex(5, 0.8f);
    /*Object p99{cubex,
            glm::vec3(1.0f, 0.8f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)};*/
        
    //cubex.loadBufferData();

    ParserXYZ parser{"../../F8BT.xyz"};
    Data d{parser};

    std::shared_ptr mesh = std::make_shared<Icosphere>(5, 0.8f);
    scene.addMesh(mesh);

    /*std::unique_ptr object = std::make_unique<Object>(mesh,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.5f, 0.5f, 0.5f));
            
    std::unique_ptr object2 = std::make_unique<Object>(mesh,
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.5f, 0.0f, 1.0f));

    scene.addObject(std::move(object));
    scene.addObject(std::move(object2));*/

    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            for (int z = 0; z < 10; ++z) {
                // Nastavenie pozície každého objektu v mriežke, centrované na (0,0,0)
                glm::vec3 position = glm::vec3((x - 4.5f) * 2.0f, (y - 4.5f) * 2.0f, (z - 4.5f) * 2.0f); // posúvame pozície pre X, Y, Z
                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

                // Nastavenie farby – variácia farby podľa pozície v mriežke
                float red = (x / 9.0f);    // Červená bude rásť od 0 do 1 v osi X
                float green = (y / 9.0f);  // Zelená bude rásť od 0 do 1 v osi Y
                float blue = (z / 9.0f);   // Modrá bude rásť od 0 do 1 v osi Z

                // Nastavenie farby
                glm::vec3 color = glm::vec3(red, green, blue);

                // Vytvorenie a pridanie objektu do scény
                auto object = std::make_unique<Object>(mesh, position, rotation, scale, color);
                scene.addObject(std::move(object));
            }
        }
    }

    scene.init();
    
    glClearColor(settings.backgroundColor.r, settings.backgroundColor.g, settings.backgroundColor.b, settings.backgroundColor.a);
    while (!glfwWindowShouldClose(glfwWindow)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.draw(shadedInstanceShader);

        /*p99.changeRotation((float)glfwGetTime());
        p99.draw(shadedShader);*/

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

Scene::Scene(Settings& settings) : settings{settings} {}

void Scene::init() {
    for (const auto& mesh : meshes) {
        std::vector<glm::mat4> modelMatrices;
        std::vector<glm::vec3> colors;

        // Získaj všetky objekty, ktoré používajú tento mesh
        for (const auto& object : objects) {
            if (object->mesh == mesh) {
                modelMatrices.push_back(object->getModelMatrix());
                colors.push_back(object->getColor());
            }
        }

        //mesh->smoothSurface();
        // Load buffer data for each mesh with instance attributes
        mesh->loadBufferData(modelMatrices, colors);
    }
}

void Scene::addMesh(std::shared_ptr<MeshType> mesh) {
    meshes.insert(mesh);
}
void Scene::addObject(std::unique_ptr<Object> object) {
    objects.push_back(std::move(object));
}

void Scene::draw(Shader& shader) {
    shader.use();

    for (const auto& mesh : meshes) {
        size_t instanceCount = 0;

        // Počet inštancií sa rovná počtu objektov s rovnakým meshom
        for (const auto& object : objects) {
            if (object->mesh == mesh) {
                instanceCount++;
            }
        }

        // Ak má mesh nejaké inštancie, vykreslíme ich
        if (instanceCount > 0) {
            mesh->drawInstances(instanceCount);
        }
    }
}

/*Scene::~Scene() {
}*/
