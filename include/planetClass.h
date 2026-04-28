#pragma once
#ifndef PLANET_CLASS_H
#define PLANET_CLASS_H

#include <vector>
#include <glm/glm.hpp>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"

class Planet
{
public:
    Planet(float radius, glm::vec3 position, int sectorCount = 36, int stackCount = 18);

    void Draw(Shader& shader);
    void Delete();

private:
    float radius;
    glm::vec3 position;

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    VAO vao;
    VBO* vbo;
    EBO* ebo;

    void generateSphere(int sectorCount, int stackCount);
};

#endif // !PLANET_CLASS_H

