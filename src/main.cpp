#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "camera.h"

/* Initialize window width and height */
const unsigned int WIDTH = 800, HEIGHT = 600;

/* Define camera settings */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

/* Mouse callback */

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = WIDTH, lastY = HEIGHT;
    static bool firstMouse = true;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessScroll((float)yoffset);
}

/* Viewport changer */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/* Input function */
void processKeyboard(GLFWwindow* window);

/* Define time variables */
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {

    if (!glfwInit()) {
        std::cout << "Failed to init GLFW\n";
        return -1;
    }

    /* Define vertices */
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.5f, // 0
        0.5f, -0.5f, 0.5f, // 1
        0.5f, 0.5f, 0.5f, // 2
        -0.5f, 0.5f, 0.5f, // 3
        -0.5f, -0.5f, -0.5f, // 4
        0.5f, -0.5f, -0.5f, // 5
        0.5f, 0.5f, -0.5f, // 6
        -0.5f, 0.5f, -0.5f // 7
    };

    /* Define indices */
    GLuint indices[] = {
    0, 1, 2,
    0, 2, 3,
    1, 2, 5, 
    2, 5, 6,
    4, 5, 6,
    4, 6, 7,
    3, 4, 7,
    0, 3, 4,
    0, 1, 5,
    0, 4, 5,
    2, 3, 6,
    3, 6, 7
    };

    /* Define cube positions */
    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f), // 0
    glm::vec3(2.0f, 5.0f, -15.0f), // 1
    glm::vec3(-1.5f, -2.2f, -2.5f), // 2
    glm::vec3(-3.8f, -2.0f, -12.3f), // 3
    glm::vec3(2.4f, -0.4f, -3.5f), // 4
    glm::vec3(-1.7f, 3.0f, -7.5f), // 5
    glm::vec3(1.3f, -2.0f, -2.5f), // 6
    glm::vec3(1.5f, 2.0f, -2.5f), // 7
    glm::vec3(1.5f, 0.2f, -1.5f), // 8
    glm::vec3(-1.3f, 1.0f, -1.5f) // 9
    };

    /* Specify glfw and OpenGL versions */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Initialize window */
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    /* Make window be current context */
    glfwMakeContextCurrent(window);

    /* Initialize mouse callback */
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Initialize glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD\n";
        return -1;
    }

    /* Enable depth */
    glEnable(GL_DEPTH_TEST);

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
        processKeyboard(window);
        
        /* Update time variables */
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /* Use shader program */
        shaderProgram.Activate();

        /* Set time unfirom */
        shaderProgram.setFloat("time", currentFrame);

        /* Set Background */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Bind VAO */
        VAO1.Bind();

        /* Create view matrix */
        glm::mat4 view = camera.GetViewMatrix();

        /* Set view uniform */
        shaderProgram.setMat4("view", view);

        /* Create projection matrix */
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f,
            100.0f);

        /* Set projection uniform */
        shaderProgram.setMat4("projection", projection);

        for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++){
            /* Create model matrix */
            glm::mat4 model = glm::mat4(1.0f);
            model= glm::translate(model, cubePositions[i]);

            model = glm::rotate(model, currentFrame * (i + 1.0f)* 0.5f,
                glm::vec3(1.0f, -1.0f, 0.0f));

            /* Set model uniform */
            shaderProgram.setMat4("model", model);

            /* Draw */
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

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

/* Keyboard function */
void processKeyboard(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}
