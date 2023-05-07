
#include <glad/glad.h> //has to be included before glfw
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "core/Log.h"
#include "Application.h"

#include <iostream>
#include <string>
#include <entt/entt.hpp>
#include "core/BaseTypes.h"

//Old way of declaring static version of the app
//is reasonable but if there is a shrd_ptr of app that needs the static instance to also be destroyed when closed, use cherno method
static Application* app = nullptr;

Application& Application::GetApp()
{
    return *app;
}

//Keep in mind a weak_ptr needs an existing shrd_ptr in order to actually be made
//static std::weak_ptr<Application> appInstance;
//
//std::shared_ptr<Application> Application::Get()
//{
//    return appInstance.lock();
//}

static void glfw_error_callback(int error, const char* description)
{
    //fprintf freaks out the compiler before init, so use cpp api 
    //fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    //const std::string s = std::to_string(error);
    std::cout << description << " :" << error << std::endl;
}
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    app->WindowSize = { width, height };
    //Application::Get()->WindowSize = {width, height};
    std::cout << "Hey man" << std::endl;
}


void Application::Run()
{
    app = this;
    log_dbg("Starting Application");
    // Setup _window
    glfwSetErrorCallback(glfw_error_callback); //Calling this somehow breaks glfwInit so for now keep commented, never mind its just glfwInit

    if (!glfwInit()) //Calling this through Local windows Debugger (F5) causes slow startup sometimes, USE Ctrl + F5 to launch without debugger for no problems
        log_error("Init wrong");
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only


       // Create _window with graphics context
    _window = glfwCreateWindow(1280, 720, "Leon Land", NULL, NULL);
    if (_window == NULL)
        log_error("No _window");

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    //https://www.glfw.org/docs/3.3/window_guide.html#window_sizelimits
    glfwSetWindowAspectRatio(_window, AspectRatio.x, AspectRatio.y);


    glfwMakeContextCurrent(_window);

    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(0); // (1) Enable vsync


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        log_error("Failed to init GLAD");
    }


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform _windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //bool show_demo__window = true;
    //bool show_another__window = false;
    _clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    OnInit();

    // Main loop
    while (!glfwWindowShouldClose(_window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
        OnDrawUI();
        ImGui::EndFrame();
        ImGui::Render();

        // Rendering
        OnRender();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(_window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(_window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(_window);
    glfwTerminate();
}

static float SPAWN_RADIUS = 100;

void Application::OnInit()
{

    const entt::entity entity = _registry.create();
    Rect lmao = Rect({ 0,0 }, { 1,1 });
    _registry.emplace<Rect>(entity, glm::vec2( 0,0 ), glm::vec2( 1,1 ));

    //Demo code for colliders
    for (int i = 0; i < 10000; i++)
    {
        //auto spawnPos = 
        auto size = glm::vec2(0.5, 0.5);
        CollisionRect obj;
        obj._rect.Bounds = size;
        auto randIndex = _textures.size();
        _colliders.push_back(obj);
        auto thing = _registry.get<Rect>(entity);

    }


    _shader = Shader::Create("shaders/vertex.glsl", "shaders/frag.glsl");
    _renderer.Init(_shader);
    auto container = Texture::GenerateTexture("assets/container.jpg");
    auto agiri = Texture::GenerateTexture("assets/agiri_christmas.jpg");
    //this is called uniform initialisation
    static Quad quads[] = {
        {{0,0}, {1,1}, 0.0, agiri.get()},
        {{1,0}, {0.6,1}, 70, container.get()}
    };
    //_quads.reserve(20); //Do this to make this process faster
    //This copies over all the values (asks for quad r value OR a reference to an existing quad)
    _quads.push_back(quads[0]);
    _quads.push_back(quads[1]);
    //emplace back needs a proper ctor to be made for it to work and pass the args into
    // emplaced back doesnt support uniform initialisation 
    // https://stackoverflow.com/questions/8782895/why-doesnt-emplace-back-use-uniform-initialization
    // This is why cpp sucks as a modern language, sometimes some things just need to be torn down from the start and rewritten 
    //_quads.emplace_back(glm::vec2(0,0), glm::vec2(1, 1), 0, agiri.get());
    // agiri->Bind(0);
    // _shader->SetUniformi("image", 0);
    // container->Bind(1);
    // _shader->SetUniformi("background", 1);

    //VERY IMPORTANT that the reference to texture doesnt get lost at the end of the scope here otherwise destructor will get run
    //If textures were referenced using quads (using shrd_ptrs) this would likely not be a necessity 
    _textures.push_back(std::move(agiri));
    _textures.push_back(std::move(container));
    ImGui::GetStyle();
}

void Application::OnDrawUI()
{
    ImGui::ShowDemoWindow();
    {
        static float f = 0.0f;
        static int counter = 0;
        static bool wireFrame = false;
        static glm::vec4 squareColor = { 0.463, 0.725, 0 , 1 };
        static char imageFile[255] = "assets/muhheart.jpg";
        static glm::vec3 position = {};
        static glm::vec3 scale(1.0f);
        static float angle = 0;
        glm::mat4 transform = glm::mat4(1.0f);

        ImGui::Begin("Change leonLand values");
        ImGui::Checkbox("Wire Frame", &wireFrame);
        ImGui::ColorEdit3("clear color", (float*)&_clear_color);
        //ImGui::ColorEdit4("SquareColor", glm::value_ptr(squareColor));
        //ImGui::DragFloat3("Position", glm::value_ptr(position));
        //ImGui::DragFloat3("Scale", glm::value_ptr(scale));
        //ImGui::DragFloat("Angle", &angle);
        ImGui::DragFloat2("Cam Position", glm::value_ptr(_cam.camRect.Pos));
        ImGui::DragFloat2("Cam Scale", glm::value_ptr(_cam.camRect.Bounds));
        ImGui::InputTextWithHint("Image file", "enter file loc here", imageFile, 255);
        ImGui::DragFloat("Spawn Radius", &SPAWN_RADIUS);

        //if (ImGui::Button("Change Picture"))
        //{
        //    glActiveTexture(GL_TEXTURE1);
        //    glBindTexture(GL_TEXTURE_2D, GenerateTexture(imageFile));
        //}

        //ImGui::SameLine();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        if (wireFrame)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }

}
//Keep this in mind to render while resizing
//https://stackoverflow.com/questions/45880238/how-to-draw-while-resizing-glfw-window

struct CollisionData
{
    entt::entity from;
    entt::entity to;
};

//The system here comprises of only 1 function but you get the point
void CollisionHandleSystem() 
{

}


void Application::OnRender()
{
    //Eval physics loop
    auto deltaTime = ImGui::GetIO().DeltaTime;

    auto view = _registry.view<CollisionRect>();
    for (auto entity : view)
    {
        CollisionRect &col = view.get<CollisionRect>(entity);
        col.isSolved = false;
    }

    

    std::vector<Entity> entitiesCollided;
    for (auto [outEnt, outCol] : view.each())
    for (auto [inEnt, inCol] : view.each())
    {
        if (inEnt == outEnt) continue;

    }
    
    //Ref<Rect> rect = std::make_shared<Rect>();


    //Clear buffer
    int display_w, display_h;
    //Can use set framebuffer size callback instyead
    glfwGetFramebufferSize(_window, &display_w, &display_h);
    //These are input values, only resize to what you want here, set min perhaps
    glViewport(0, 0, display_w, display_h);
    glClearColor(_clear_color.x * _clear_color.w, _clear_color.y * _clear_color.w, _clear_color.z * _clear_color.w, _clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    _renderer.BeginBatch();

    _shader->SetMat4("cam", _cam.GetMat4());

    for (Quad& q : _quads)
    {
        _renderer.AddQuad(q);
    }
    _renderer.EndBatch();

}
