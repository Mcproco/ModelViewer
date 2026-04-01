#define GLAD_GL_IMPLEMENTATION
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <stdbool.h>
#include <stdlib.h>
#include <cimgui.h>
#include <cimgui_impl.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
const char* TITLE = "HERROW WORLD!";
const float verticies[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
};

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true); 
    }
}

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    igCreateContext(NULL);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(NULL);
    igStyleColorsDark(NULL);

    bool showWindow = true;

    while (!glfwWindowShouldClose(window)) {

        processInput(window);
        glfwSwapBuffers(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        igNewFrame();

        igShowDemoWindow(&showWindow);
        igRender();
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

        glfwPollEvents();
    }

    glfwTerminate();
    
    return EXIT_SUCCESS;
}
