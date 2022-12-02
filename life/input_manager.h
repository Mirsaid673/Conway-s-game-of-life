#pragma once

#include "../Engine/Core/application.h"

#include "glob.h"

class InputManager : public Node
{
public:
    float camera_scale = 10.0f;

    void update() override
    {
        if (input.getMouseButtonPress(GLFW_MOUSE_BUTTON_LEFT))
        {
            glm::ivec2 cursor_world = glm::round(getCursorWorld(input.getCursorPos()) + world.getSize() / 2.0f);
            if (cursor_world.x >= 0 && cursor_world.x < world.getWidth() &&
                cursor_world.y >= 0 && cursor_world.y < world.getHeight())
            {
                world.flipValue(cursor_world);
            }
        }

        if (input.getMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
        {
            glm::vec2 cursor_offset = input.getCursorOffset();
            if (cursor_offset != glm::vec2(0))
                camera2d.transform.origin -= getCursorWorldOffset(cursor_offset);
        }

        int scroll = input.getMouseScroll();
        if (scroll)
        {
            camera_scale -= scroll * camera_scale * 0.1f;
            camera_scale = glm::clamp(camera_scale, 2.0f, 1000.0f);
            resizeCamera();
        }
    }

    glm::vec2 getCursorWorld(const glm::vec2 &cursor)
    {
        float w = window.getWidth();
        float h = window.getHeight();

        float x = (cursor.x / w - 0.5f) * camera2d.getWidth();
        float y = -(cursor.y / h - 0.5f) * camera2d.getHeight();

        return glm::vec2(x, y) + camera2d.transform.origin;
    }

    glm::vec2 getCursorWorldOffset(const glm::vec2 &offset)
    {
        float w = window.getWidth();
        float h = window.getHeight();

        float x = camera2d.getWidth() * offset.x / w;
        float y = camera2d.getHeight() * offset.y / h;
        return glm::vec2(x, -y);
    }

    void resizeCamera()
    {
        float w = window.getWidth();
        float h = window.getHeight();
        float asp;
        if (w > h)
        {
            asp = w / h * camera_scale;
            camera2d.ortho(-asp, asp, camera_scale, -camera_scale);
        }
        else
        {
            asp = h / w * camera_scale;
            camera2d.ortho(-camera_scale, camera_scale, asp, -asp);
        }
    }
};