#include "starClass.h"

Star::Star(float radius, float mass, glm::vec3 position, glm::vec3 color)
    : Planet(radius, mass, position, color), lightColor(color) {
    isLightSource = true;
}

void Star::ApplyLight(Shader& shader) {
    shader.setVec3("lightPos", GetPosition());
    shader.setVec3("lightColor", lightColor);
}

void Star::ApplyForce(glm::vec3 force, float dt) {
    // intentional no-op
}