#pragma once

#include "../Engine/Core/node.h"
#include <box2d/box2d.h>

class Physics : public Node
{
private:
    b2World world;
    int velocityIterations = 6;
    int positionIterations = 2;

public:
    Physics() : world({0.0f, -20.0f}) {}

    b2Body *createBody(const b2BodyDef *def)
    {
        return world.CreateBody(def);
    }

    void update() override
    {
        world.Step(Time::delta_time, velocityIterations, positionIterations);
    }
} physics;