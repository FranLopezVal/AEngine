#include "AEWindow.h"
#include "Libs/imgui_internal.h"

std::vector<Node*> nodes;

    
AEWindow::AEWindow()
{
#ifdef DEBUG
    AEWindow::isDebug = true;
#endif // DEBUG

}

AEWindow::~AEWindow()
{
    SDL_DestroyRenderer(_renderer_);
    SDL_DestroyWindow(_window_);
    SDL_Quit();

    //Debug::Warning("End AEngine.");

}

void AEWindow::Run()
{
    // initiate SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("[ERROR] %s\n", SDL_GetError());
        return;
    }

    // set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    std::string glsl_version = "#version 150";
#ifdef __APPLE__
    // GL 3.2 AEWindow + GLSL 150
    glsl_version = "#version 150";
    SDL_GL_SetAttribute( // required on Mac OS
        SDL_GL_CONTEXT_FLAGS,
        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#elif __linux__
    // GL 3.2 AEWindow + GLSL 150
    glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif _WIN32
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

    _appEventHandler->CallOnInit(); //EVENT------------------------------------

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(
        SDL_WINDOW_OPENGL
        | SDL_WINDOW_RESIZABLE
        | SDL_WINDOW_ALLOW_HIGHDPI
        );
    SDL_Window* window = SDL_CreateWindow(
        "Dear ImGui SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        window_flags
    );
    //std::unique_ptr<AEManager> am = std::make_unique<AEManager>();
   
    // limit to which minimum size user can resize the window
    SDL_SetWindowMinimumSize(window, 500, 300);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);

    // enable VSync
    SDL_GL_SetSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "[ERROR] Couldn't initialize glad" << std::endl;
    }
    else
    {
        std::cout << "[INFO] glad initialized\n";
    }

    glViewport(0, 0, windowWidth, windowHeight);

    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
   
    // setup Dear ImGui style
    ImGui::StyleColorsDark();

    // setup platform/renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    StyleColorAngy(NULL);

    ImVec4 background = ImVec4(85 / 255.0f, 35 / 255.0f, 35 / 255.0f, 1.00f);

    glClearColor(background.x, background.y, background.z, background.w);
    
    // Create some nodes
    nodes.push_back(new Node(0, V2(100, 100)));
    nodes.push_back(new Node(1, V2(200, 200)));
    nodes.push_back(new Node(1, V2(200, 100)));


    bool __run = true;
    while (__run)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            // ImGui SDL input
            ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
            ImGuiIO& io = ImGui::GetIO();

            // Handle core SDL events (close window, key pressed, etc.)
            switch (sdlEvent.type) {
            case SDL_QUIT:
                __run = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                //
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    __run = false;
                }
            }
        }


        // start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();


        //_appEventHandler->CallOnAppUpdate(); //EVENT------------------------------------
        /*
        {
            ImGui::NewFrame();
            static int counter = 0;
            // get the window size as a base for calculating widgets geometry
            int sdl_width = 0, sdl_height = 0, controls_width = 0;
            SDL_GetWindowSize(window, &sdl_width, &sdl_height);
            controls_width = sdl_width;
            // make controls widget width to be 1/3 of the main window width
            if ((controls_width /= 3) < 300) { controls_width = 300; }

            // position the controls widget in the top-right corner with some margin
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
            // here we set the calculated width and also make the height to be
            // be the height of the main window also with some margin
            ImGui::SetNextWindowSize(
                ImVec2(static_cast<float>(controls_width), static_cast<float>(sdl_height - 20)),
                ImGuiCond_Always
            );
            // create a window and append into it
            ImGui::Begin("Tool Box", NULL, ImGuiWindowFlags_NoResize);

            //ImGui::Dummy(ImVec2(0.0f, 1.0f));
            //ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
            //ImGui::Text("%s", SDL_GetPlatform());
            //ImGui::Text("CPU AEWindows: %d", SDL_GetCPUCount());
            //ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);

            const char* names[2] = { "Nodes", "Logic"};
            static bool opened[2] = { true, true }; // Persistent user state

            for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
            {
                if (n > 0) { ImGui::SameLine(); }
                ImGui::Checkbox(names[n], &opened[n]);
            }

            if (ImGui::BeginTabBar("ToolBoxTabBar")) //tab_bar_flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
                    if (opened[n] && ImGui::BeginTabItem(names[n], &opened[n], ImGuiTabItemFlags_None))
                    {
                        ImGui::Text("This is the %s tab!", names[n]);
                        if (n & 1)
                            ImGui::Text("I am an odd tab.");
                        ImGui::EndTabItem();
                    }
                ImGui::EndTabBar();
            }
            ImGui::Separator();


            ImGui::SameLine();
           // ImGui::Text("counter = %d", counter);


            ImGui::End();
        }
        */

        ImGui::Begin("Canvas");
        ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));
        ImGui::SetWindowPos(ImVec2(0, 0));

        // Dibujar los nodos en el canvas
        ImGui::BeginChild("CanvasArea", ImVec2(0, 0), true);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));

        for (auto& node : nodes) {
            ImGui::PushID(node->id);
            ImVec2 node_rect_min = node->handle_position.toIgV2();
            ImVec2 node_rect_max = ImVec2(Vector2(node->handle_position + V2(150, 100)).toIgV2());
            ImGui::SetCursorScreenPos(node_rect_min);
            Vector2 a = Vector2().StaticfromIgV2(node_rect_max) - Vector2().StaticfromIgV2(node_rect_min);

            ImGui::Button("node", a.toIgV2());
            if (ImGui::IsItemHovered()) {
                ImGui::SetHoveredID(node->id);
                if (ImGui::IsMouseClicked(0)) {
                    node->selected = true;
                }
            }
            if (node->selected) {
                ImGui::BeginGroup();
                ImGui::Text("Node %d", node->id);
                ImGui::EndGroup();
            }

            ImGui::SameLine();
            ImGui::Button("delete", ImVec2(16, 16));
            if (ImGui::IsItemClicked()) {
                nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
                ImGui::PopID();
                continue;
            }
            ImGui::PopID();
        }
        ImGui::PopStyleVar(2);
        ImGui::EndChild();

        // Manejar la creación y movimiento de los nodos
        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemActive()) {
            ImVec2 mp = ImGui::GetMousePos();
            ImVec2 gdd = ImGui::GetMouseDragDelta(0);
            ImVec2 click_pos = Vector2(Vector2().StaticfromIgV2(mp)
                * Vector2().StaticfromIgV2(gdd)).toIgV2();
           
            int idx = static_cast<int>(nodes.size());
            
            Node* n = new Node(idx, Vector2().StaticfromIgV2(click_pos), false);
            nodes.push_back(n);
        }

        if (ImGui::IsAnyItemHovered() && ImGui::IsMouseDragging(0)) {
            ImVec2 delta = ImGui::GetMousePos();
            for (auto& node : nodes) {
                if (node->selected) {
                    node->handle_position = Vector2().StaticfromIgV2(delta);
                }
            }
        }
    

        ImGui::End();

        // Renderizar IMGUI
        ImGui::Render();
        SDL_GL_MakeCurrent(window, gl_context);
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    for (auto& node : nodes)
    {
        delete[] node;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // ...
}


