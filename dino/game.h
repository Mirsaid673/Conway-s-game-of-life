#pragma once

#include "../Engine/Core/application.h"
#include "ground.h"
#include "dino.h"
#include "physics.h"

class Game : public Application
{
    Ground ground;
    Dino dino;
    void init() override
    {
        camera2d.ortho((float)window.getWidth() / (float)window.getHeight() * scale, scale);
        Renderer::setClearColor(glm::vec3(1));
        scene.addNode(physics);
        scene.addNode(ground);
        scene.addNode(dino);
    }

    float scale = 10.0f;
    void onResize() override
    {
        camera2d.ortho((float)window.getWidth() / (float)window.getHeight() * scale, scale);
    }

    void update() override
    {
        if (input.getKeyDown(GLFW_KEY_ESCAPE))
            running = false;
    }
};