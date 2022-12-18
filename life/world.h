#pragma once

#include "../Engine/Core/application.h"
#include <vector>
#include <glm/glm.hpp>
#include "theme.h"

class World : public Node
{
public:
    Framebuffer field;
    Framebuffer buffer;

    float tps = 2.0f;
    float elapsed = 0.0f;
    bool pause = true;
    bool grid_enable = true;

    size_t width;
    size_t height;

    void tick()
    {
        Renderer::pushDrawTarget(buffer);

        calculate_program.use();
        field.getTexture().use();
        Renderer::drawVAO(GPU::gpu.getDeafultQuad());

        Renderer::popDrawTarget();

        std::swap(buffer, field);
    }

    void update() override
    {
        if (pause)
            return;

        elapsed += Time::delta_time;
        if (elapsed >= 1 / tps)
        {
            elapsed = 0.0f;
            tick();
        }
    }

    void draw() override
    {
        Transform2d transform;
        transform.scale(glm::vec2(width, height));
        transform.origin = glm::vec2(-0.5f);

        program.use();
        program.setVec3("alive_color", Theme::alive_color);
        program.setVec3("dead_color", Theme::dead_color);
        field.getTexture().use();
        program.setPVM(camera2d.getPV() * transform.getMatrix());
        Renderer::drawVAO(quad);

        if (grid_enable)
            drawGrid();
    }

    void drawGrid()
    {
        float size = Theme::grid_width;

        Program::colored2d.use();
        Program::colored2d.vertexAttrib(VertexAttribs::COLOR, Theme::grid_color);
        Transform2d transform;
        transform.scale({size, camera2d.getHeight()});

        glm::ivec2 min = glm::floor(camera2d.transform.origin - camera2d.getSize() / 2.0f);
        glm::ivec2 max = glm::ceil(camera2d.transform.origin + camera2d.getSize() / 2.0f);

        for (int x = min.x; x <= max.x; x++)
        {
            transform.origin = glm::vec2(x - 0.5f, camera2d.transform.origin.y);
            Program::colored2d.setPVM(camera2d.getPV() * transform.getMatrix());
            Renderer::drawVAO(GPU::gpu.getDeafultQuad());
        }

        Transform2d transform2;
        transform2.scale({camera2d.getWidth(), size});
        for (int y = min.y; y <= max.y; y++)
        {
            transform2.origin = glm::vec2(camera2d.transform.origin.x, y - 0.5f);
            Program::colored2d.setPVM(camera2d.getPV() * transform2.getMatrix());
            Renderer::drawVAO(GPU::gpu.getDeafultQuad());
        }
    }

public:
    void init() override
    {
        int new_width = width / 8;
        Image img;
        img.width = new_width;
        img.height = height;
        img.format = Image::Format::RED;
        img.data_type = Image::DataType::UNSIGNED_BYTE;
        Texture::InternalFormat in_f = Texture::InternalFormat::UINTEGER;

        field.create(img, Framebuffer::Type::TEXRUE, in_f);
        buffer.create(img, Framebuffer::Type::TEXRUE, in_f);
        field.getTexture().filter(Texture::Filter::NEAREST);
        buffer.getTexture().filter(Texture::Filter::NEAREST);
        field.getTexture().wrapMode(Texture::WrapMode::REPEAT);
        buffer.getTexture().wrapMode(Texture::WrapMode::REPEAT);

        clear();
        buffer.bind();
        Renderer::clearBuffers();
        Framebuffer::bindMain();
    }
    World(size_t w, size_t h) : width(w), height(h) {}
    ~World()
    {
        field.destroy();
        buffer.destroy();
    }

    void flipValue(const glm::ivec2 &coord)
    {
        uint8_t data;
        glm::ivec2 new_coord = glm::ivec2(coord.x / 8, coord.y);
        field.readPixels(new_coord, glm::ivec2(1), &data);

        int bit = coord.x % 8;
        flipBit(data, bit);

        field.getTexture().subimage(new_coord, glm::ivec2(1), &data);
    }

    void clear()
    {
        field.bind();
        Renderer::clearBuffers();
        field.unbind();
    }
    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }
    glm::vec2 getSize() const { return {width, height}; }
    void flipBit(uint8_t &data, int bit)
    {
        uint8_t mask = 1 << bit;
        data ^= mask;
    }

public:
    static Program program;
    static Program calculate_program;
    static RID quad;
    static void initialize()
    {
        program.create("../Engine/openGL/Program/Shaders/2d/basic.vert", "../life/resources/shader.frag");
        calculate_program.create("../Engine/openGL/Program/Shaders/framebuffer/basic.vert", "../life/resources/calculate_shader.frag");
        quad = GPU::gpu.loadMesh(ResourceManager::createQuadMesh(glm::vec2(-0.5), glm::vec2(0.5)));
    }

    static void determinate()
    {
        program.destroy();
        calculate_program.destroy();
    }
};

Program World::program;
Program World::calculate_program;
RID World::quad;