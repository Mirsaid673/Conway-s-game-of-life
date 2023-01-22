#pragma once

#include "../Engine/Core/node.h"

class Ground : public Node
{
private:
    RID quad;
    Material m;

public:
    Ground()
    {
        quad = GPU::gpu.getDeafultQuad();    
    }
    void init() override
    {
    }
    void update() override {}
    void draw() override
    {
        //Renderer::setMaterial(m);
        Renderer::drawVAO(quad);
    }
};