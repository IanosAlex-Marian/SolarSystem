#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "camera.h"
#include "planetClass.h"
#include "starClass.h"
#include "gravitySystem.h"

const unsigned int WIDTH = 800, HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 30.0f, 80.0f));

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = WIDTH / 2.0f, lastY = HEIGHT / 2.0f;
    static bool firstMouse = true;
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos; lastY = ypos;
    camera.ProcessMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessScroll((float)yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processKeyboard(GLFWwindow* window);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool gravityEnabled = true;

int main() {
    if (!glfwInit()) { std::cout << "Failed to init GLFW\n"; return -1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System", NULL, NULL);
    if (!window) { std::cout << "Failed to create window\n"; glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD\n"; return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shaderProgram(
        std::string(SHADER_DIR) + "default.vert",
        std::string(SHADER_DIR) + "default.frag"
    );

    GravitySystem gravitySystem;

    // Sun: fixed in place via Star::ApplyForce no-op
    Star* sun = new Star(3.0f, 5000.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.9f, 0.7f));

    // Circular orbit velocity: v = sqrt(G * M_sun / r), G=3, M_sun=5000
    /* All planet masses set to 1.0 so planet - planet forces are negligible */
    const float G = 3.0f;
    const float M = 5000.0f;

    Planet* venus = new Planet(0.7f, 1.0f, glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.9f, 0.7f, 0.5f));
    Planet* earth = new Planet(0.8f, 1.0f, glm::vec3(8.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.5f, 1.0f));
    Planet* mars = new Planet(0.6f, 1.0f, glm::vec3(12.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.4f, 0.2f));
    Planet* jupiter = new Planet(1.5f, 1.0f, glm::vec3(18.0f, 0.0f, 0.0f), glm::vec3(0.7f, 0.5f, 0.3f));
    Planet* mercury = new Planet(0.5f, 1.0f, glm::vec3(3.5f, 0.0f, 0.0f), glm::vec3(0.7f, 0.6f, 0.5f));
    Planet* saturn = new Planet(1.4f, 1.0f, glm::vec3(24.0f, 0.0f, 0.0f), glm::vec3(0.9f, 0.8f, 0.6f));
    Planet* uranus = new Planet(1.2f, 1.0f, glm::vec3(30.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.8f, 0.9f));
    Planet* neptune = new Planet(1.2f, 1.0f, glm::vec3(36.0f, 0.0f, 0.0f), glm::vec3(0.3f, 0.5f, 0.9f));
    
    venus->SetVelocity(glm::vec3(0.0f, 0.0f, sqrtf(G * M / 5.0f)));   // ~54.77
    earth->SetVelocity(glm::vec3(0.0f, 0.0f, sqrtf(G * M / 8.0f)));   // ~43.30
    mars->SetVelocity(glm::vec3(0.0f, 0.0f, sqrtf(G * M / 12.0f)));  // ~35.36
    jupiter->SetVelocity(glm::vec3(0.0f, 0.0f, sqrtf(G * M / 18.0f)));  // ~28.87
    neptune->SetVelocity(glm::vec3(0.0f, 0.0f, sqrtf(G * M / 36.0f)));   // ~20.41
    uranus->SetVelocity(glm::vec3(0.0f, 0.0f, sqrtf(G * M / 30.0f)));   // ~22.36
    saturn->SetVelocity(glm::vec3(0.0f, 0.0f, sqrtf(G * M / 24.0f)));   // ~25.0
    mercury->SetVelocity(glm::vec3(0.0f, 0.0f, sqrtf(G * M / 3.5f)));   // ~65.47
    
    gravitySystem.AddBody(sun);
    gravitySystem.AddBody(venus);
    gravitySystem.AddBody(earth);
    gravitySystem.AddBody(mars);
    gravitySystem.AddBody(jupiter);
    gravitySystem.AddBody(mercury);
    gravitySystem.AddBody(saturn);
    gravitySystem.AddBody(uranus);
    gravitySystem.AddBody(neptune);

    std::vector<Planet*> planets = { sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune };

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 1000.0f);

    while (!glfwWindowShouldClose(window)) {
        processKeyboard(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (deltaTime > 0.033f) deltaTime = 0.033f;

        if (gravityEnabled) {
            const int substeps = 10;
            float subDt = deltaTime / substeps;
            for (int i = 0; i < substeps; ++i) {
                gravitySystem.CalculateAndApplyForces(subDt);
                gravitySystem.UpdatePositions(subDt);
            }
        }

        shaderProgram.Activate();
        glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);

        sun->ApplyLight(shaderProgram);
        shaderProgram.setVec3("ambientLight", glm::vec3(0.25f, 0.25f, 0.3f));

        for (auto& planet : planets)
            planet->Draw(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderProgram.Delete();
    for (auto& planet : planets) { planet->Delete(); delete planet; }
    gravitySystem.ClearBodies();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processKeyboard(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        static bool gPressed = false;
        if (!gPressed) {
            gravityEnabled = !gravityEnabled;
            std::cout << "Gravity " << (gravityEnabled ? "enabled" : "disabled") << std::endl;
            gPressed = true;
        }
    }
    else {
        static bool gPressed = false;
        gPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        static bool rPressed = false;
        if (!rPressed) {
            camera = Camera(glm::vec3(0.0f, 10.0f, 15.0f));
            rPressed = true;
        }
    }
    else {
        static bool rPressed = false;
        rPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)       camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime);
}