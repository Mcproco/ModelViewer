#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#include <stdbool.h>
#include <stdlib.h>
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
const char* TITLE = "HERROW WORLD!";

const float vertices[] = {
//   X         Y         Z          R       G       B
     0.5f,     0.5f,     0.0f,      1.0f,   0.0f,   0.0f,       1.0f, 1.0f,
     0.5f,    -0.5f,     0.0f,      0.0f,   1.0f,   0.0f,       1.0f, 0.0f,
    -0.5f,    -0.5f,     0.0f,      0.0f,   0.0f,   1.0f,       0.0f, 0.0f,
    -0.5f,     0.5f,     0.0f,      0.0f,   0.0f,   1.0f,       0.0f, 1.0f,
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

void initTexture(unsigned int *texture) {

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("./res/container.jpg", &width, &height, &nrChannels, 0);

    if (data) {

        GLint mipmap_level = 0;
        GLint internal_format = GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, mipmap_level, internal_format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    } else {
        std::cout << "Fuck this language bro" << "\n";
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void processFrameBuffer(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
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
    glfwSetFramebufferSizeCallback(window, processFrameBuffer);
    gladLoadGL(glfwGetProcAddress);

    unsigned int texture;
    initTexture(&texture);

    ShaderProgram shaderProgram;
    ShaderGenProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl", &shaderProgram);
    ShaderUseProgram(shaderProgram);

    unsigned int vao, vbo, ebo;
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

            /*define the VBO's element count, data, and storage type*/
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); //DO NOT unbind the EBO while the VAO is active, as it will unbind the EBO from the VAO
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //define the EBO's size, data and storage type (indicies)

    glBindVertexArray(0);
    glUniform1i(ShaderUniformLocation("Texture0", shaderProgram), GL_TEXTURE0); /* Set the proper uniform sampler2D variable to the right texture unit */
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //For the FRONT and BACK of every triangle, render primative as a line
    

    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);

        ShaderUseProgram(shaderProgram);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::rotate(transform, glm::radians((float)glfwGetTime() * 20.0f), glm::vec3(0.0f, 0.0f, 1.0f));

            glUniformMatrix4fv(ShaderUniformLocation("transform", shaderProgram), 1, GL_FALSE, glm::value_ptr(transform));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    
    return EXIT_SUCCESS;
}
