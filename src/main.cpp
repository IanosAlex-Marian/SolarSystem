#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

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

    /* Define shaders */
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    /* Define program and add shaders to it*/

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    /* Delete shaders */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /* Initialize viewport changer to framebuffer*/
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Define VAO */

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* Define VBO */
    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Define EBO */
    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   

    /* Unbind our buffers */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Render loop */
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        /* Set Background */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Use shader program */
        glUseProgram(shaderProgram);

        /* Bind VAO */
        glBindVertexArray(VAO);
        
        /* Draw */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap Front & Back */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* Delete all */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
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
