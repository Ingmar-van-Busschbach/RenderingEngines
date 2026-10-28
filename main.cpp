#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/Objects/camera.h"
#include "core/Objects/Light.h"
#include "core/UserConfig.h"
#include "core/texture.h"
#include "core/Rendering/ModelRenderer.h"

//#define MAC_CLION
#define VSTUDIO

#ifdef MAC_CLION
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#ifdef VSTUDIO
// Note: install imgui with:
//     ./vcpkg.exe install imgui[glfw-binding,opengl3-binding]
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

int g_width = 800;
int g_height = 600;

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void processCameraInput(GLFWwindow *window, core::objects::Camera &camera, float deltaTime)
{
    float cameraSpeed = core::UserConfig::cameraTranslationSpeed;
    float rotationSpeed = core::UserConfig::cameraRotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        cameraSpeed *= core::UserConfig::cameraBoostMultiplier;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.Translate(camera.cameraForward * deltaTime * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.Translate(-camera.cameraForward * deltaTime * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.Translate(camera.cameraRight * deltaTime * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.Translate(-camera.cameraRight * deltaTime * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.Translate(camera.cameraUp * deltaTime * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.Translate(-camera.cameraUp * deltaTime * cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        camera.Rotate(-rotationSpeed * deltaTime, 0, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        camera.Rotate(rotationSpeed * deltaTime, 0, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        camera.Rotate(0, -rotationSpeed * deltaTime, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        camera.Rotate(0, rotationSpeed * deltaTime, 0);
    }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    g_width = width;
    g_height = height;
    glViewport(0, 0, width, height);
}

std::string readFileToString(const std::string &filePath) {
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        printf("Could not open file: %s\n", filePath.c_str());
        return "";
    }
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

GLuint generateShader(const std::string &shaderPath, GLuint shaderType) {
    printf("Loading shader: %s\n", shaderPath.c_str());
    const std::string shaderText = readFileToString(shaderPath);
    const GLuint shader = glCreateShader(shaderType);
    const char *s_str = shaderText.c_str();
    glShaderSource(shader, 1, &s_str, nullptr);
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Error! Shader issue [%s]: %s\n", shaderPath.c_str(), infoLog);
    }
    return shader;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //Setup platforms
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const GLuint modelVertexShader = generateShader("shaders/modelVertex.vs", GL_VERTEX_SHADER);
    const GLuint vertexShader = generateShader("shaders/vertex.vs", GL_VERTEX_SHADER);
    const GLuint fragmentShader = generateShader("shaders/fragment.fs", GL_FRAGMENT_SHADER);
    const GLuint textureShader = generateShader("shaders/texture.fs", GL_FRAGMENT_SHADER);

    core::rendering::ModelRenderer modelRenderer;
    modelRenderer.GenerateShaderProgram(modelVertexShader, textureShader);
    modelRenderer.GenerateShaderProgram(vertexShader, fragmentShader);

    glDeleteShader(modelVertexShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(textureShader);

    core::Mesh quad = core::Mesh::generateQuad();
    core::Model quadModel({quad});
    quadModel.translate(glm::vec3(0,0,-2.5));
    quadModel.scale(glm::vec3(5, 5, 1));

    core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    core::Model suzanne2 = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    suzanne2.translate(glm::vec3(6, 0,0));
    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);


    core::objects::Camera camera = core::objects::Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

    //Generate Lights
    std::vector<core::objects::Light*> lights;
    lights.push_back(new core::objects::Light(core::objects::Directional, glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0, 1, 0), glm::vec4(1,1,1,1), 0.3f, glm::vec3(0,0,0),0,0));
    lights.push_back(new core::objects::Light(core::objects::Point, glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0), glm::vec4(1,0,0,1), 0.3f, glm::vec3(0,0,0),0,0));
    lights.push_back(new core::objects::Light(core::objects::Spot, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0, 0, 1), glm::vec4(0,1,0,1), 0.3f, glm::vec3(0,0,0),5,3));
    lights.push_back(new core::objects::Light(core::objects::Ambient, glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(-1, -1, -1), glm::vec4(0,0,1,1),0.1f, glm::vec3(0,0,0),0,0));


    // Load Light Shader Data
    modelRenderer.GenerateShaderDatas(lights.size());

    double currentTime = glfwGetTime();
    double finishFrameTime = 0.0;
    float deltaTime = 0.0f;
    float rotationStrength = 100.0f;
    bool faceTarget = false;
















    //FrameLoop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::Begin("Raw Engine v2");
        //ImGui::Text("Hello :)");
        //ImGui::End();

        processCameraInput(window, camera, deltaTime);
        processInput(window);
        suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));
        modelRenderer.ApplyLightShaderData(lights);
        modelRenderer.Render(quadModel, 1, camera.modelMatrix, projection, cmgtGatoTexture);
        modelRenderer.Render(suzanne2, 1, camera.modelMatrix, projection, cmgtGatoTexture);
        modelRenderer.Render(suzanne, 1, camera.modelMatrix, projection, cmgtGatoTexture);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
        finishFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(finishFrameTime - currentTime);
        currentTime = finishFrameTime;
    }






    //End
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}