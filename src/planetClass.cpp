#include "planetClass.h"

/* Constructor */
Planet::Planet(float radius, glm::vec3 position, int sectors, int stacks)
    : radius(radius), position(position){
    generateSphere(sectors, stacks);

    vao.Bind();

    vbo = new VBO(vertices.data(), vertices.size() * sizeof(GLfloat));
    ebo = new EBO(indices.data(), indices.size() * sizeof(GLuint));

    vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    vao.Unbind();
}

/* Use model with global position */
void Planet::Draw(Shader& shader){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    shader.setMat4("model", model);

    vao.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Planet::generateSphere(int sectors, int stacks)
{
    float x, y, z;
    float sectorStep = 2 * glm::pi<float>() / sectors;
    float stackStep = glm::pi<float>() / stacks;

    /* Create vertices */
    for (int i = 0; i <= stacks; ++i)
    {
        float stackAngle = glm::pi<float>() / 2 - i * stackStep;
        float xy = radius * cos(stackAngle);
        z = radius * sin(stackAngle);

        for (int j = 0; j <= sectors; ++j)
        {
            float sectorAngle = j * sectorStep;

            x = xy * cos(sectorAngle);
            y = xy * sin(sectorAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    /* Create indices */
    for (int i = 0; i < stacks; ++i)
    {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

/* Delete function*/
void Planet::Delete() {
    vao.Delete();
    vbo->Delete();
    ebo->Delete();

    delete vbo;
    delete ebo;
}