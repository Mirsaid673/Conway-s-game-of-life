#pragma once

#include "../Engine/Core/node.h"
#include "physics.h"

class Dino : public Node
{
private:
    RID quad;
    Material m;
    Transform2d transform;
    b2Body *body;
    bool grounded = true;

public:
    Dino()
    {
        glm::vec2 pos = {-3, 0};
        glm::vec2 size = {1, 1};

        m.program = &Program::basic2d;
        Image::setFlipOnLoad(true);
        m.texture = GPU::gpu.createTexture("../dino/resource/hero.jpg");
        GPU::gpu.textureFilter(m.texture, Texture::Filter::NEAREST);
        GPU::gpu.textureWrapMode(m.texture, Texture::WrapMode::MIRRORED_REPEAT);
        quad = GPU::gpu.getDeafultQuad();

        transform.origin = pos;
        transform.scale(size);

        b2BodyDef def;
        def.fixedRotation = true;
        def.position.Set(pos.x, pos.y);
        def.type = b2_dynamicBody;

        b2PolygonShape shape;
        shape.SetAsBox(size.x, size.y);

        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density = 1.0f;
        fixture.friction = 0.3f;

        body = physics.createBody(&def);
    
        body->CreateFixture(&fixture);
        b2MassData m;
        m.mass = 1;
        m.center = {0, 0};
        m.I = 0;
        body->SetMassData(&m);
    }

    void update() override
    {
        b2Vec2 pos = body->GetPosition();
        transform.origin = {pos.x, pos.y};

        if (input.getKeyDown(GLFW_KEY_SPACE) && grounded)
        {
            grounded = false;
            spdlog::info("space pressed");
            body->ApplyForceToCenter({0, 1000}, true);
        }
    }
    void draw() override
    {

        Renderer::setMaterial(m);
        Renderer::setTransform2d(transform.getMatrix());

        Renderer::drawVAO(quad);
    }
};