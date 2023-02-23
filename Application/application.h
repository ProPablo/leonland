#pragma once
#include "imgui/imgui.h"
#include "rendering/Shader.h"
#include "rendering/Quad.h"
#include "rendering/Renderer.h"
#include "rendering/Camera.h"

//Has to be included after everything
#include <GLFW/glfw3.h>
#include "collision/Quadtree.h"

class Application
{
public:
    static Application& GetApp();
    //https://www.youtube.com/watch?v=lats2jd-JB8 for recs on how this was made
    //static std::shared_ptr<Application> Get();
    glm::uvec2 WindowSize;
    glm::uvec2 AspectRatio = { 16,9 };

    void Run();
private:

    std::vector<Quad> _quads;
    GLFWwindow* _window = nullptr;
    ImVec4 _clear_color = {};
    Renderer _renderer;
    //orthographic projection will make it so z doesnt matter here
    //Currently, all positions go from -1 to 1
    //glm::vec3 camPosition = {0.0f, 0.0f, 3.0f};
    Camera _cam;


    void OnDrawUI();
    void OnRender();
    void OnInit();
    std::shared_ptr<Shader> _shader;
    std::vector<std::unique_ptr<Texture>> _textures;
    std::vector<CollisionRect> _colliders;

};

