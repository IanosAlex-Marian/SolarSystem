#include "gravitySystem.h"

void GravitySystem::AddBody(Planet* body) {
    bodies.push_back(body);
}

void GravitySystem::RemoveBody(Planet* body) {
    auto it = std::find(bodies.begin(), bodies.end(), body);
    if (it != bodies.end()) {
        bodies.erase(it);
    }
}

void GravitySystem::ClearBodies() {
    bodies.clear();
}

void GravitySystem::CalculateAndApplyForces(float dt) {
    std::vector<glm::vec3> forces(bodies.size(), glm::vec3(0.0f));
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            glm::vec3 forceOnJ = CalculateGravitationalForce(*bodies[i], *bodies[j]);
            forces[j] += forceOnJ;
            forces[i] -= forceOnJ;
        }
    }

    for (size_t i = 0; i < bodies.size(); ++i) {
        bodies[i]->ApplyForce(forces[i], dt);
    }
}

void GravitySystem::UpdatePositions(float dt) {
    for (auto& body : bodies) {
        body->Update(dt);
    }
}