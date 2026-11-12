#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include "core/Data/mesh.h"
#include "core/assimpLoader.h"
#include "core/Scene.h"
#include "core/Objects/camera.h"
#include "core/Objects/Light.h"
#include "core/UserConfig.h"
#include "core/Data/texture.h"
#include "core/Objects/SceneObject.h"
#include "core/Rendering/ModelRenderer.h"
#include "core/Rendering/SkyboxRenderer.h"

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

#ifdef _WIN32
#include <windows.h>
extern "C" {
// Force NVIDIA drivers to use the dedicated GPU
_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
// Force AMD drivers to use the dedicated GPU
_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
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

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "RenderingEngine", NULL, NULL);
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
    const GLuint skyboxVertexShader = generateShader("shaders/skybox.vs", GL_VERTEX_SHADER);
    const GLuint skyboxFragmentShader = generateShader("shaders/skybox.fs", GL_FRAGMENT_SHADER);
    const GLuint pbrFragmentShader = generateShader("shaders/pbr.fs", GL_FRAGMENT_SHADER);
    const GLuint waterVertexShader = generateShader("shaders/water.vs", GL_VERTEX_SHADER);
    const GLuint waterFragmentShader = generateShader("shaders/water.fs", GL_FRAGMENT_SHADER);

    core::rendering::ModelRenderer modelRenderer;
    modelRenderer.GenerateShaderProgram(skyboxVertexShader, skyboxFragmentShader);
    modelRenderer.GenerateShaderProgram(vertexShader, fragmentShader);
    modelRenderer.GenerateShaderProgram(vertexShader, pbrFragmentShader);
    modelRenderer.GenerateShaderProgram(waterVertexShader, waterFragmentShader);

    glDeleteShader(modelVertexShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(textureShader);
    glDeleteShader(skyboxVertexShader);
    glDeleteShader(skyboxFragmentShader);

    core::rendering::SkyboxRenderer skyboxRenderer(modelRenderer.shaderPrograms[0]);

    core::Mesh skyBox = core::Mesh::generateSkyBox();

    std::string facesCubemap[6] =
    {
        "textures/right.jpg",
        "textures/left.png",
        "textures/top.png",
        "textures/bottom.png",
        "textures/front.png",
        "textures/back.png"
    };

    core::Texture skyBoxTexture(facesCubemap);

    core::Mesh quad = core::Mesh::generateQuad();
    core::Model quadModel({quad});
    quadModel.translate(glm::vec3(0,-2.5,0));
    quadModel.rotate(glm::vec3(1,0,0), glm::radians(-90.0f));
    quadModel.scale(glm::vec3(5, 5, 1));

    core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    suzanne.translate(glm::vec3(0,3,0));

    core::Model suzanne2 = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    suzanne2.translate(glm::vec3(6, 0,0));

    core::Model suzanne3 = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    suzanne3.translate(glm::vec3(-4, 0,0));

    core::Model spaceship = core::AssimpLoader::loadModel("models/spaceship.obj");
    spaceship.translate(glm::vec3(0,0,0));

    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");

    core::Texture waterTexture("textures/water.jpg");

    core::Texture crystalAlbedoTexture("textures/Crystals_base_color.png");
    core::Texture crystalSpecularTexture("textures/Crystals_specular_roughness.png");
    core::Texture crystalNormalTexture("textures/Crystals_normal.png");

    core::Texture tilesAlbedoTexture("textures/Tiles_base_color.png");
    core::Texture tilesSpecularTexture("textures/Tiles_specular_roughness.png");
    core::Texture tilesMetalTexture("textures/Tiles_MetallicSmoothness.png");
    core::Texture tilesNormalTexture("textures/Tiles_normal.png");

    core::Texture spaceshipAlbedoTexture("textures/spaceship_albedo.jpg");
    core::Texture spaceshipMetallicTexture("textures/spaceship_metallic.jpg");
    core::Texture spaceshipRoughnessTexture("textures/spaceship_roughness.jpg");
    core::Texture spaceshipNormalTexture("textures/spaceship_normal.jpg");
    core::Texture spaceShipAOTexture("textures/spaceship_ao.jpg");
    core::Texture spaceShipEmissiveTexture("textures/spaceship_emissive.jpg");

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,clearColor.g, clearColor.b, clearColor.a);

    core::objects::Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

    //Generate Lights
    std::vector<core::objects::Light*> lights;
    lights.push_back(new core::objects::Light(core::objects::Directional, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0), glm::vec4(1,1,1,1), 0.3f, glm::vec3(0,0,0),0,0));
    lights.push_back(new core::objects::Light(core::objects::Point, glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0), glm::vec4(1,0,0,1), 0.3f, glm::vec3(0,0.05f,0.05f),0,0));
    lights.push_back(new core::objects::Light(core::objects::Spot, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0, 0, 1), glm::vec4(0,1,0,1), 0.3f, glm::vec3(0,0.05f,0.05f),2,1));
    lights.push_back(new core::objects::Light(core::objects::Ambient, glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(-1, -1, -1), glm::vec4(0,0,1,1),0.1f, glm::vec3(0,0,0),0,0));

    std::vector<core::objects::Light*> lights2;
    lights2.push_back(new core::objects::Light(core::objects::Directional, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 1), glm::vec4(1,1,1,1), 1.0f, glm::vec3(0,0,0),0,0));
    lights2.push_back(new core::objects::Light(core::objects::Point, glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0), glm::vec4(1,1,0,1), 0.3f, glm::vec3(0,0.05f,0.05f),0,0));
    lights2.push_back(new core::objects::Light(core::objects::Spot, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0, 0, 1), glm::vec4(1,0,1,1), 0.3f, glm::vec3(0,0.05f,0.05f),5,3));
    lights2.push_back(new core::objects::Light(core::objects::Ambient, glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(-1, -1, -1), glm::vec4(0,1,1,1),0.1f, glm::vec3(0,0,0),0,0));

    //Generate Materials
    std::vector<core::data::Material*> materials;
    materials.push_back(new core::data::Material(core::data::ADSWithTextures, crystalAlbedoTexture.getId(), crystalSpecularTexture.getId(), crystalNormalTexture.getId(), 0, 0 ,0,0, glm::vec3(1,1,1), 0.5f, 0 , 0));
    materials.push_back(new core::data::Material(core::data::ADSWithTextures, tilesAlbedoTexture.getId(), tilesSpecularTexture.getId(), tilesNormalTexture.getId(), 0 , 0,0,0, glm::vec3(1,1,1), 0.5f, 0 , 0));
    materials.push_back(new core::data::Material(core::data::ADSWithOnlyAlbedoTexture, cmgtGatoTexture.getId(), tilesSpecularTexture.getId(), tilesNormalTexture.getId(), 0 , 0,0,0, glm::vec3(1,1,1), 0.5f, 0 , 0));
    materials.push_back(new core::data::Material(core::data::ADSWithTextures, spaceshipAlbedoTexture.getId(), spaceshipMetallicTexture.getId(), spaceshipNormalTexture.getId(), 0 , 0,0,0, glm::vec3(1,1,1), 0.5f, 0 , 0));
    materials.push_back(new core::data::Material(core::data::PBRWithTextures, spaceshipAlbedoTexture.getId(), 0, spaceshipNormalTexture.getId(), spaceshipMetallicTexture.getId() , spaceshipRoughnessTexture.getId(), spaceShipAOTexture.getId(), spaceShipEmissiveTexture.getId(),glm::vec3(1,1,1), 0.5f, 0 , 0));
    materials.push_back(new core::data::Material(core::data::ADSWithOnlyAlbedoTexture, waterTexture.getId(), tilesSpecularTexture.getId(), tilesNormalTexture.getId(), 0 , 0,0,0, glm::vec3(1,1,1), 0.5f, 0 , 0));

    std::vector<core::objects::SceneObject*> sceneObjects;
    sceneObjects.push_back(new core::objects::SceneObject(&quadModel,materials.at(5), 3));
    sceneObjects.push_back(new core::objects::SceneObject(&suzanne,materials.at(1), 1));
    sceneObjects.push_back(new core::objects::SceneObject(&spaceship,materials.at(4), 2));

    std::vector<core::objects::SceneObject*> sceneObjects2;
    sceneObjects2.push_back(new core::objects::SceneObject(&quadModel,materials.at(1), 1));
    sceneObjects2.push_back(new core::objects::SceneObject(&suzanne2,materials.at(2), 1));
    sceneObjects2.push_back(new core::objects::SceneObject(&suzanne3,materials.at(0), 1));
    sceneObjects2.push_back(new core::objects::SceneObject(&spaceship,materials.at(3), 1));

    std::vector<core::Scene*> scenes;
    scenes.push_back(new core::Scene(sceneObjects, lights));
    scenes.push_back(new core::Scene(sceneObjects2, lights2));

    // Load Light Shader Data
    modelRenderer.GenerateShaderDatas(scenes.at(0)->lights.size());
    modelRenderer.ApplyLightShaderData(scenes.at(0)->lights);


    double currentTime = glfwGetTime();
    double finishFrameTime = 0.0;
    float deltaTime = 0.0f;
    float rotationStrength = 100.0f;
    bool faceTarget = false;

    int sceneIndex = 0;
    //FrameLoop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Scene Selector");
        if (ImGui::Button("Switch Scene"))
        {
            sceneIndex++;
            if (sceneIndex >= scenes.size())
            {
                sceneIndex = 0;
            }
            modelRenderer.GenerateShaderDatas(scenes.at(sceneIndex)->lights.size());
        }
        ImGui::End();
        processCameraInput(window, camera, deltaTime);
        processInput(window);
        //quadModel.rotate(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));
        suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));
        lights.at(0)->SetDirection(glm::vec3(glm::sin(currentTime), 2.0f, glm::cos(currentTime)));
        lights.at(2)->SetDirection(glm::vec3(-glm::cos(currentTime), 0.0f, glm::sin(currentTime)));
        modelRenderer.ApplyTimeShaderData(static_cast<float>(currentTime), deltaTime);
        modelRenderer.ApplyCamaraShaderData(camera.cameraPos);
        modelRenderer.ApplyLightShaderData(scenes.at(sceneIndex)->lights);
        for (core::objects::SceneObject* sceneObject : scenes.at(sceneIndex)->sceneObjects)
        {
            modelRenderer.Render(sceneObject->model, sceneObject->shaderProgramIndex, camera.modelMatrix, projection, sceneObject->material);
        }
        skyboxRenderer.ApplyMatrices(camera.modelMatrix, projection);
        skyboxRenderer.Render(skyBox, skyBoxTexture);
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

    for (int i = 0; i < lights.size(); i++)
    {
        delete(lights.at(i));
    }
    for (int i = 0; i < lights2.size(); i++)
    {
        delete(lights2.at(i));
    }
    for (int i = 0; i < materials.size(); i++)
    {
        delete(materials.at(i));
    }
    for (int i = 0; i < sceneObjects.size(); i++)
    {
        delete(sceneObjects.at(i));
    }
    for (int i = 0; i < sceneObjects2.size(); i++)
    {
        delete(sceneObjects2.at(i));
    }
    for (int i = 0; i < scenes.size(); i++)
    {
        delete(scenes.at(i));
    }
    lights.clear();
    lights2.clear();
    materials.clear();
    sceneObjects.clear();
    sceneObjects2.clear();
    scenes.clear();

    glfwTerminate();
    return 0;
}