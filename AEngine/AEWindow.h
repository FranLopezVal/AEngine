#pragma once
#ifndef AEWindow_H
#define AEWindow_H

#define _MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define _MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define _CLAMP(VALUE, MN, MX)     ((VALUE) < (MN) ? (MN) : (VALUE) > (MX) ? (MX) : (VALUE))


#include <iostream>
#include <stdio.h>
#include <string>

#include <SDL.h>
#include <SDL_opengles2.h>
#include "Libs/imgui.h"
#include "Libs/imgui_impl_opengl3.h"
#include "Libs/imgui_impl_sdl.h"
#include "Libs/glad.h"
#include "Node.hpp"
#include "AEEvent.h"



const int TARGET_FPS = 60;
const int MS_PER_FRAME = 100 / TARGET_FPS;

struct Screen
{
    int x;
    int y;
    short deep_bit = 0x10; //16 bits
};



class AEWindow
{
    //override de clases de copia, no dejara que esta clase sea copiada, solo referenciada.
    AEWindow(AEWindow&&) = delete;
    AEWindow(AEWindow const&) = delete;
    AEWindow& operator=(AEWindow&&) = delete;
    AEWindow& operator=(AEWindow const&) = delete;

private:


    bool isRunning{ false };
    bool isDebug{ false };
    int millisecondsLastFrame{ 0 };


    int windowWidth = 1280,
        windowHeight = 720;


    SDL_Window* _window_{ nullptr };
    SDL_Renderer* _renderer_{ nullptr };
    SDL_Rect _camera_{ 0,0,100,100 };

    std::unique_ptr<Screen> CurrentScreen{ std::make_unique<Screen>() };


    const ImVec4 Hex2V4(std::string hex)
    {
        size_t size = hex.length();
        float* colors = new float[3];
        int f_counter = 0;

        if (!(size > 5 && size < 8))return ImVec4(.0f, .0f, .0f, .0f);

        for (int i = (hex[0] == '#' || hex[0] == 'h'/* ||hex[0] == 'H'*/) ? 1 : 0; i < size; i += 2)
        {
            colors[f_counter++] = (float)std::stoul(hex.substr(i, 2), nullptr, 16) / 255;
            //f_counter++;
        }

        return ImVec4(colors[0], colors[1], colors[2], 1.0f);
    }


#define CL_LIME Hex2V4("276440") // Hex2V4("#a3C01F");
#define CL_ORANGE CL_LIME;//Hex2V4("#ffa401");
#define CL_BG_GRAY Hex2V4("2a2a2c");
#define CL_PNL_GRAY Hex2V4("3a3f3c");
#define CL_LG_GRAY Hex2V4("33423E");

#define CL_LIGHT Hex2V4("D8DDE3");
#define CL_LIGHT_SHADOW Hex2V4("C1B99B");
#define CL_MID_LIGHT Hex2V4("7E7E87");


    inline void StyleColorAngy(ImGuiStyle* dst)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("./Resources/Mono.ttf", 15.0f);

        ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
        ImVec4* colors = ImGui::GetStyle().Colors;
        // ROJO VERDE AZUL ALPHA
        colors[ImGuiCol_Text] = CL_LIGHT;
        colors[ImGuiCol_TextDisabled] = CL_LIGHT_SHADOW;
        colors[ImGuiCol_WindowBg] = CL_BG_GRAY;
        colors[ImGuiCol_ChildBg] = CL_PNL_GRAY;
        colors[ImGuiCol_PopupBg] = CL_PNL_GRAY;
        colors[ImGuiCol_Border] = CL_ORANGE;
        colors[ImGuiCol_BorderShadow] = CL_LIGHT_SHADOW;
        colors[ImGuiCol_FrameBg] = CL_BG_GRAY;
        colors[ImGuiCol_FrameBgHovered] = CL_LG_GRAY;
        colors[ImGuiCol_FrameBgActive] = CL_BG_GRAY;
        colors[ImGuiCol_TitleBg] = CL_BG_GRAY;
        colors[ImGuiCol_TitleBgActive] = CL_BG_GRAY;
        colors[ImGuiCol_TitleBgCollapsed] = CL_ORANGE;
        colors[ImGuiCol_MenuBarBg] = CL_BG_GRAY;
        colors[ImGuiCol_ScrollbarBg] = CL_BG_GRAY;
        colors[ImGuiCol_ScrollbarGrab] = CL_LG_GRAY;
        colors[ImGuiCol_ScrollbarGrabHovered] = CL_MID_LIGHT;
        colors[ImGuiCol_ScrollbarGrabActive] = CL_ORANGE;
        colors[ImGuiCol_CheckMark] = CL_ORANGE;
        colors[ImGuiCol_SliderGrab] = CL_MID_LIGHT;
        colors[ImGuiCol_SliderGrabActive] = CL_ORANGE;
        colors[ImGuiCol_Button] = CL_LG_GRAY;
        colors[ImGuiCol_ButtonHovered] = CL_ORANGE;
        colors[ImGuiCol_ButtonActive] = CL_MID_LIGHT;
        colors[ImGuiCol_Header] = CL_LG_GRAY;
        colors[ImGuiCol_HeaderHovered] = CL_ORANGE;
        colors[ImGuiCol_HeaderActive] = CL_BG_GRAY;
        colors[ImGuiCol_Separator] = CL_ORANGE;
        colors[ImGuiCol_SeparatorHovered] = CL_ORANGE;
        colors[ImGuiCol_SeparatorActive] = CL_LIGHT;
        colors[ImGuiCol_ResizeGrip] = CL_MID_LIGHT;
        colors[ImGuiCol_ResizeGripHovered] = CL_LIGHT;
        colors[ImGuiCol_ResizeGripActive] = CL_LIGHT_SHADOW;
        colors[ImGuiCol_Tab] = CL_LG_GRAY;
        colors[ImGuiCol_TabHovered] = CL_ORANGE;
        colors[ImGuiCol_TabActive] = CL_BG_GRAY;
        colors[ImGuiCol_TabUnfocused] = CL_BG_GRAY;
        colors[ImGuiCol_TabUnfocusedActive] = CL_LG_GRAY;
        colors[ImGuiCol_PlotLines] = CL_ORANGE;
        colors[ImGuiCol_PlotLinesHovered] = CL_ORANGE;
        colors[ImGuiCol_PlotHistogram] = CL_LIME;
        colors[ImGuiCol_PlotHistogramHovered] = CL_LIME;
        colors[ImGuiCol_TableHeaderBg] = CL_BG_GRAY;
        colors[ImGuiCol_TableBorderStrong] = CL_LG_GRAY;  // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableBorderLight] = CL_MID_LIGHT;   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableRowBg] = CL_LG_GRAY;
        colors[ImGuiCol_TableRowBgAlt] = CL_MID_LIGHT;
        colors[ImGuiCol_TextSelectedBg] = CL_ORANGE;
        colors[ImGuiCol_DragDropTarget] = CL_ORANGE;
        colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);


        ImGui::GetStyle().WindowBorderSize = 0.0f;
        ImGui::GetStyle().WindowRounding = 12.0f;
        ImGui::GetStyle().ChildRounding = 12.0f;
        ImGui::GetStyle().FrameRounding = 12.0f;
        ImGui::GetStyle().PopupRounding = 12.0f;
        ImGui::GetStyle().ScrollbarRounding = 12.0f;
        ImGui::GetStyle().GrabRounding = 12.0f;
        ImGui::GetStyle().TabRounding = 12.0f;
    }

public:
    AEWindow();
    ~AEWindow();

    Application* _appEventHandler = new Application();

    

    static bool* can_add_object;

    bool IsDebug() { return isDebug; };

    void Run();
    
};


#endif // !M_AEWindow