#include "application.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

void init_glad();
void init_opengl();
GLFWwindow *init_glfw(int width, int height, int x_pos, int y_pos);
void init_imgui(GLFWwindow *glfw_window);
void glfw_error_callback(int error, const char *description);
void loop(GLFWwindow *glfw_window, GUI::Application *app);
void destroy(GLFWwindow *glfw_window);

int main()
{
    srand(time(NULL));
    // srand(114514);
    auto glfw_window = init_glfw(800, 600, 0, 0);
    init_glad();
    init_opengl();
    auto app = GUI::Application();
    glfwSetWindowUserPointer(glfw_window, &app);
    // dispatch_events(glfw_window, imgui_window.get());

    init_imgui(glfw_window);
    // register callbacks

    // maximize the window
    app.init();
    glfwMaximizeWindow(glfw_window);

    // GUI LOOP
    loop(glfw_window, &app);

    // CLEANUP
    destroy(glfw_window);
    return 0;
}

/*--------------------------------------------------------
                        implementation
----------------------------------------------------------*/

void init_glad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

void init_opengl()
{
    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    // enable multisampling
    glEnable(GL_MULTISAMPLE);
    // enable face culling
    glEnable(GL_CULL_FACE);
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLFWwindow *init_glfw(int width, int height, int x_pos, int y_pos)
{
    GLFWwindow *glfw_window = nullptr;
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfw_window = glfwCreateWindow(width, height, "GUI", NULL, NULL);
    if (!glfw_window)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwSetWindowPos(glfw_window, x_pos, y_pos);
    glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(1);
    // enable multisampling
    glfwWindowHint(GLFW_SAMPLES, 4);
    return glfw_window;
}

void init_imgui(GLFWwindow *glfw_window)
{
    // get the window size
    int width, height;
    glfwGetWindowSize(glfw_window, &width, &height);
    // get the window position
    int x_pos, y_pos;
    glfwGetWindowPos(glfw_window, &x_pos, &y_pos);
    // init imgui

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // enable docking
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void glfw_error_callback(int error, const char *description)
{
    std::cout << "GLFW ERROR: " << error << " " << description << std::endl;
}

void loop(GLFWwindow *glfw_window, GUI::Application *app)
{
    while (!glfwWindowShouldClose(glfw_window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto viewport = ImGui::GetMainViewport();
        ImGui::DockSpaceOverViewport(viewport, ImGuiDockNodeFlags_PassthruCentralNode);

        app->run();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (ImGui::GetIO().ConfigFlags)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(glfw_window);
    }
}

void destroy(GLFWwindow *glfw_window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
    std::cout << "QUIT" << std::endl;
    Rendering::shader_program_factory.destroy();
}