#pragma once

#include "../Engine/Core/application.h"
#include <vector>
#include <glm/glm.hpp>

class World : public Node
{
public:
    friend class Game;
    float tps = 2.0f;
    float elapsed = 0.0f;
    bool pause = true;
    bool grid_enable = true;

    std::vector<std::vector<bool>> field;
    std::vector<std::vector<bool>> buffer;
    size_t width;
    size_t height;

    void tick()
    {
        for (size_t x = 0; x < width; x++)
        {
            for (size_t y = 0; y < height; y++)
            {
                glm::ivec2 current_coord(x, y);
                int alive_neighs = getAliveNeighboursCount(current_coord);
                if (getCell(current_coord))
                {
                    if (alive_neighs == 2 || alive_neighs == 3)
                        setCellBuffer(current_coord, true);
                }
                else
                {
                    if (alive_neighs == 3)
                        setCellBuffer(current_coord, true);
                }
            }
        }
        clear();
        std::swap(field, buffer);
    }

    int getAliveNeighboursCount(const glm::ivec2 &coord)
    {
        static const glm::ivec2 offsets[8] =
            {
                glm::ivec2(-1, -1),
                glm::ivec2(-1, 0),
                glm::ivec2(-1, 1),
                glm::ivec2(1, -1),
                glm::ivec2(1, 0),
                glm::ivec2(1, 1),
                glm::ivec2(0, -1),
                glm::ivec2(0, 1),
            };

        int count = 0;
        for (auto &offset : offsets)
            count += getCell(coord + offset);

        return count;
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

        Program::colored2d.use();
        Program::colored2d.vertexAttrib(VertexAttribs::COLOR, glm::vec3(0));

        for (size_t x = 0; x < width; x++)
        {
            for (size_t y = 0; y < height; y++)
            {
                if (field[x][y])
                {
                    transform.origin = glm::vec2(x, y) - glm::vec2(width, height) / 2.0f;
                    Program::colored2d.setPVM(camera2d.getPV() * transform.getMatrix());
                    Renderer::drawVAO(quad);
                }
            }
        }
        if (grid_enable)
            drawGrid();
    }

    void drawGrid()
    {
        float size = 0.05f;

        Program::colored2d.vertexAttrib(VertexAttribs::COLOR, glm::vec3(0.5f));
        Transform2d transform;
        transform.scale({size, camera2d.getHeight()});

        glm::ivec2 min = glm::floor(camera2d.transform.origin - camera2d.getSize() / 2.0f);
        glm::ivec2 max = glm::floor(camera2d.transform.origin + camera2d.getSize() / 2.0f);

        for (int x = min.x; x <= max.x; x++)
        {
            transform.origin = glm::vec2(x - 0.5f, camera2d.transform.origin.y);
            Program::colored2d.setPVM(camera2d.getPV() * transform.getMatrix());
            Renderer::drawVAO(quad);
        }

        Transform2d transform2;
        transform2.scale({camera2d.getWidth(), size});
        for (int y = min.y; y <= max.y; y++)
        {
            transform2.origin = glm::vec2(camera2d.transform.origin.x, y - 0.5f);
            Program::colored2d.setPVM(camera2d.getPV() * transform2.getMatrix());
            Renderer::drawVAO(quad);
        }
    }

public:
    World(size_t w, size_t h) : width(w), height(h), field(w, std::vector<bool>(h, false)), buffer(w, std::vector<bool>(h, false)) {}

    size_t mod(int a, size_t b)
    {
        return a - (b * glm::floor((float)a / (float)b));
    }

    void clear()
    {
        for (size_t x = 0; x < width; x++)
            for (size_t y = 0; y < height; y++)
                field[x][y] = false;
    }
    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }
    glm::vec2 getSize() const { return {width, height}; }
    int getCell(const glm::ivec2 &coord) { return static_cast<int>(field[mod(coord.x, width)][mod(coord.y, height)]); }
    void setCellBuffer(const glm::ivec2 &coord, bool value) { buffer[coord.x][coord.y] = value; }
    void flipValue(const glm::ivec2 &coord) { field[coord.x][coord.y] = !field[coord.x][coord.y]; }

    static RID quad;
    static void initialize()
    {
        quad = GPU::gpu.loadMesh(ResourceManager::createQuadMesh(glm::vec2(-0.5f, -0.5f), glm::vec2(0.5f, 0.5f)));
    }
};

RID World::quad;