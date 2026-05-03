#ifndef PLANET_CLASS_H
#define PLANET_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Planet {
public:
    Planet(float radius, float mass, glm::vec3 position, glm::vec3 color, int sectors = 36, int stacks = 36);

    void Draw(Shader& shader);
    void Update(float dt);
    virtual void ApplyForce(glm::vec3 force, float dt);
    void Delete();

    // Getters
    glm::vec3 GetPosition() const;
    glm::vec3 GetVelocity() const { return velocity; }
    float GetMass() const;
    float GetRadius() const { return radius; }

    // Setters
    void SetVelocity(glm::vec3 vel) { velocity = vel; }
    void SetPosition(glm::vec3 pos) { position = pos; }

    bool isLightSource = false;

protected:
    float radius;
    float mass;
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 velocity = glm::vec3(0.0f);

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    VAO vao;
    VBO* vbo;
    EBO* ebo;

    void generateSphere(int sectors, int stacks);
};

// Gravity function declaration
glm::vec3 CalculateGravitationalForce(const Planet& a, const Planet& b);

#endif