#pragma once

#include "glob.h"
#include "world.h"
#include "imgui_layer.h"
#include "input_manager.h"

class Game : public Application
{
    ImguiLayer imlayer;
    InputManager im;

    void init() override
    {
        World::initialize();
        Renderer::setClearColor({1, 1, 1, 1});
        float asp = (float)window.getWidth() / (float)window.getHeight() * im.camera_scale;
        camera2d.ortho(-asp, asp, im.camera_scale, -im.camera_scale);
        camera2d.transform.origin = glm::vec2(0.5f);

        scene.addNode(im);
        scene.addNode(world);
        scene.addNode(imlayer);
        //world.field[4][2] = world.field[5][1] = world.field[3][0] = world.field[4][0] = world.field[5][0] = true;
    }

    void onResize() override
    {
        im.resizeCamera();
    }

    void update() override
    {
        if (input.getKeyDown(GLFW_KEY_ESCAPE))
            running = false;
    }
};