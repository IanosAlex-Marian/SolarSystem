#ifndef STAR_CLASS_H
#define STAR_CLASS_H

#include "planetClass.h"
#include "shaderClass.h"

class Star : public Planet {
public:
    Star(float radius, float mass, glm::vec3 position, glm::vec3 color);
    void ApplyForce(glm::vec3 force, float dt) override;
    void ApplyLight(Shader& shader);

private:
    glm::vec3 lightColor;
};

#endif