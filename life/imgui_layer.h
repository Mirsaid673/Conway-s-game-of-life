#pragma once

#include "../Engine/Core/application.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "glob.h"

class ImguiLayer : public Node
{
public:
    ImGuiIO *io;

    void init() override
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = &ImGui::GetIO();
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        ImGui::StyleColorsDark();
        ImGuiStyle &style = ImGui::GetStyle();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
    }

    void draw() override
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::Begin("control");

        if (ImGui::Button("tick"))
            world.tick();
        ImGui::SameLine();

        ImGui::Checkbox("pause", &world.pause);
        ImGui::SameLine();

        ImGui::Checkbox("grid", &world.grid_enable);
        ImGui::SameLine();

        ImGui::SliderFloat("tps", &world.tps, 0.001, 50);
        ImGui::SameLine();

        if (ImGui::Button("clear"))
            world.clear();

        ImGui::End();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void destroy() override
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
};