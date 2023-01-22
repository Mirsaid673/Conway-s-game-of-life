#pragma once

#include "../Engine/Core/application.h"
#include "ground.h"

class Game : public Application
{
    Ground ground;
    void init() override
    {
        camera2d.ortho(800, 600);
        Renderer::setClearColor(glm::vec3(1));
        scene.addNode(ground);
    }

    void update() override
    {
        if (input.getKeyDown(GLFW_KEY_ESCAPE))
            running = false;
    }
};