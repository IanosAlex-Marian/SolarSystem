#include "planetClass.h"

/* Constructor */
Planet::Planet(float radius, float mass, glm::vec3 position, glm::vec3 color, int sectors, int stacks)
    : radius(radius), mass(mass), position(position), color(color) {
    generateSphere(sectors, stacks);

    vao.Bind();

    vbo = new VBO(vertices.data(), vertices.size() * sizeof(GLfloat));
    ebo = new EBO(indices.data(), indices.size() * sizeof(GLuint));

    vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    vao.Unbind();
}

/* Use model with global position */
void Planet::Draw(Shader& shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    shader.setMat4("model", model);
    shader.setVec3("objectColor", color);
    shader.setBool("isLightSource", isLightSource);

    vao.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Planet::generateSphere(int sectors, int stacks) {
    float x, y, z;
    float sectorStep = 2 * glm::pi<float>() / sectors;
    float stackStep = glm::pi<float>() / stacks;

    /* Create vertices */
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = glm::pi<float>() / 2 - i * stackStep;
        float xy = radius * cos(stackAngle);
        z = radius * sin(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;

            x = xy * cos(sectorAngle);
            y = xy * sin(sectorAngle);

            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }
    }

    /* Create indices */
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

/* Gravity force calculation */
glm::vec3 CalculateGravitationalForce(const Planet& a, const Planet& b) {
    const float G = 3.0f;

    glm::vec3 direction = a.GetPosition() - b.GetPosition();
    float distance = glm::length(direction);
    if (distance < 0.001f) distance = 0.001f;

    float forceMagnitude = G * a.GetMass() * b.GetMass() / (distance * distance);
    float minDist = (a.GetRadius() + b.GetRadius()) * 1.2f;
    return glm::normalize(direction) * forceMagnitude;
}

/* Apply force */
void Planet::ApplyForce(glm::vec3 force, float dt) {
    glm::vec3 acceleration = force / mass;
    velocity += acceleration * dt;
}

/* Updater */
void Planet::Update(float dt) {
    position += velocity * dt;
}

/* Delete function */
void Planet::Delete() {
    vao.Delete();
    if (vbo) {
        vbo->Delete();
        delete vbo;
        vbo = nullptr;
    }
    if (ebo) {
        ebo->Delete();
        delete ebo;
        ebo = nullptr;
    }
}

/* Getters */
glm::vec3 Planet::GetPosition() const {
    return position;
}

float Planet::GetMass() const {
    return mass;
}