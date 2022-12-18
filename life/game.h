#pragma once

#include "glob.h"
#include "imgui_layer.h"
#include "input_manager.h"
#include "world.h"

class Game : public Application
{
    ImguiLayer imlayer;
    InputManager im;

    void init() override
    {
        World::initialize();
        Renderer::setClearColor(glm::vec3(0));
        float asp = (float)window.getWidth() / (float)window.getHeight() * im.camera_scale;
        camera2d.ortho(-asp, asp, im.camera_scale, -im.camera_scale);
        camera2d.transform.origin = glm::vec2(0.5f);

        scene.addNode(im);
        scene.addNode(world);
        scene.addNode(imlayer);
    }

    void determinate() override
    {
        World::determinate();
    }

    void update() override
    {
        if (input.getKeyDown(GLFW_KEY_ESCAPE))
            running = false;
    }

    void onResize() override
    {
        im.resizeCamera();
    }
};