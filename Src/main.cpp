#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <WP_Graphics/WP_Shader.h>
#include <WP_Graphics/WP_Primitive.h> 
#include <gtc/matrix_transform.hpp>
#include <WP_Scene/WP_Scene.h>
#include <WP_Scene/WP_DefaultScene.h>
#include <WP_Scene/WP_DefaultParticleScene.h>
#include <WP_Scene/WP_ImageScene.h>
#include <WP_Scene/WP_ImageArrayScene.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    const char* glsl_version = "#version 130";


    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    WP_SceneManager::GetInstance().AddScene("DefaultScene", std::make_unique<WP_DefaultScene>());
    WP_SceneManager::GetInstance().AddScene("DefaultParticleScene", 
        std::make_unique<WP_DefaultParticleScene>());
    WP_SceneManager::GetInstance().AddScene("DefaultImageScene",
        std::make_unique<WP_ImageScene>());
    WP_SceneManager::GetInstance().AddScene("DefaultImageArrayScene",
        std::make_unique<WP_ImageArrayScene>());


    // NOTE: OpenGL error checks have been omitted for brevity


    //const GLuint program = WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultShader");

    WP_SceneManager::GetInstance().RunPreSceneInitialisations();

    Primitives::Primitive::CompilePrimitives();

    float temp = 0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();


        WP_SceneManager::GetInstance().StartScene();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("FPS Display");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        WP_SceneManager::GetInstance().RunSceneSelect();

        WP_SceneManager::GetInstance().RenderSceneImGui();

        // Rendering
        ImGui::Render();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);


        WP_SceneManager::GetInstance().UpdateScene();


        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    Primitives::Primitive::CleanPrimitives();

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}