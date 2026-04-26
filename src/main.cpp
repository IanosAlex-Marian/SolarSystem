#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <direct.h>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

/* Viewport changer */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/* Input function */
void processInput(GLFWwindow* window);

int main() {

    if (!glfwInit()) {
        std::cout << "Failed to init GLFW\n";
        return -1;
    }

    /* Define vertices */
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom left
        0.5f, -0.5f, 0.0f, // Bottom right
        0.5f, 0.5f, 0.0f, // Top Right
        -0.5f, 0.5f, 0.0f // Top Left
    };

    /* Define indices */
    GLuint indices[] = {
        0,1,2, // Bottom Right
        0,2,3 // Top Left
    };

    /* Specify glfw and OpenGL versions */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Initialize window width and height */
    const unsigned int WIDTH = 800, HEIGHT = 800;

    /* Initialize window */
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    /* Make window be current context */
    glfwMakeContextCurrent(window);

    /* Initialize glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD\n";
        return -1;
    }

    /* Initialize viewport changer to framebuffer*/
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Define Shader Program */
    Shader shaderProgram(
        std::string(SHADER_DIR) + "default.vert",
        std::string(SHADER_DIR) + "default.frag"
    );

    /* Define VAO */
    VAO VAO1;
    VAO1.Bind();

    /* Define VBO */

    VBO VBO1(vertices, sizeof(vertices));

    /* Define EBO */
    EBO EBO1(indices, sizeof(indices));

    /* Link */
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    /* Unbind our buffers */
    VAO1.Unbind();
    VBO1.Unbind();

    /* Render loop */
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        /* Set Background */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Use shader program */
        shaderProgram.Activate();

        /* Bind VAO */
        VAO1.Bind();
        
        /* Draw */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap Front & Back */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* Delete all */
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    glfwTerminate();
    return 0;
}

/* Viewport changer */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/* Input function*/
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
