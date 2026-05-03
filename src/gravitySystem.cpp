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

    // Calculate all pairwise gravitational forces.
    // No mass-based skipping — the sun must participate so planets are pulled toward it.
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            glm::vec3 forceOnJ = CalculateGravitationalForce(*bodies[i], *bodies[j]);
            forces[j] += forceOnJ;
            forces[i] -= forceOnJ;
        }
    }

    // Apply forces. Star::ApplyForce is a no-op, so the sun stays fixed.
    for (size_t i = 0; i < bodies.size(); ++i) {
        bodies[i]->ApplyForce(forces[i], dt);
    }
}

void GravitySystem::UpdatePositions(float dt) {
    for (auto& body : bodies) {
        body->Update(dt);
    }
}