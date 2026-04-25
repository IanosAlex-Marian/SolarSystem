#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Solar System", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD\n";
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}