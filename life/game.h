#pragma once

#include "../Engine/Core/application.h"

class Game : public Application
{
    void init() override
    {
        Renderer::setClearColor({1, 1, 1, 1});
    }

    void update() override
    {
        if(input.getKeyDown(GLFW_KEY_ESCAPE))
            running = false;
    }
};