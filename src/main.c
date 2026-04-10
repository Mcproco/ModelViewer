#define GLAD_GL_IMPLEMENTATION
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <stdbool.h>
#include <stdlib.h>
#include <cimgui.h>
#include <cimgui_impl.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "shader.h"

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
const char* TITLE = "HERROW WORLD!";

const float vertices[] = {
//   X         Y         Z          R       G       B
    -0.5f,    -0.5f,     0.0f,      1.0f,   0.0f,   0.0f,
     0.5f,    -0.5f,     0.0f,      0.0f,   1.0f,   0.0f,
     0.0f,     0.5f,     0.0f,      0.0f,   0.0f,   1.0f,
};

const unsigned int indices[] = {
0, 1, 3,
1, 2, 3
};

unsigned int genShaderProgram(const char *vertexSrc, const char *fragmentSrc) {

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void init() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true); 
    }

}

int main() {

    init();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    //unsigned int shaderProgram = genShaderProgram(vertexShaderSource, fragmentShaderSource);
    ShaderProgram shaderProgram;
    ShaderGenProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl", &shaderProgram);
    unsigned int vao, vbo, ebo;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //For the FRONT and BACK of every triangle, render primative as a line
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //define the VBO's size, data, and storage type

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //define how the VBO is organized
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //define how the VBO is organized
            glEnableVertexAttribArray(1);
            
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); //DO NOT unbind the EBO while the VAO is active, as it will unbind the EBO from the VAO
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //define the EBO's size, data and storage type (indicies)

    glBindVertexArray(0);

    ShaderUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);

        ShaderUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    
    return EXIT_SUCCESS;
}
