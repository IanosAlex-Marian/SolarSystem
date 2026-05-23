#ifndef GRAVITY_SYSTEM_H
#define GRAVITY_SYSTEM_H

#include <vector>
#include <algorithm>
#include "planetClass.h"

class GravitySystem {
private:
    std::vector<Planet*> bodies;

public:
    void AddBody(Planet* body);
    void RemoveBody(Planet* body);
    void ClearBodies();

    void CalculateAndApplyForces(float dt);
    void UpdatePositions(float dt);
};

#endif