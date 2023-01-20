#pragma once
#include "imgui/imgui.h"
#include "rendering/Shader.h"
#include "rendering/Quad.h"
#include "rendering/Renderer.h"

//Has to be included after everything
#include <GLFW/glfw3.h>

class Application
{
public:
	void Run();
private:

	std::vector<Quad> _quads;
	GLFWwindow* _window = nullptr;
	ImVec4 _clear_color = {};
	Renderer _renderer;
	//orthographic projection will make it so z doesnt matter here
	//Currently, all positions go from -1 to 1
	//glm::vec3 camPosition = {0.0f, 0.0f, 3.0f};
	//This will represent what the outerBounds of the scale should be
	//glm::vec3 camZoom = {0.0f, 0.0f, 3.0f};

	Rect camRect;

	void OnDrawUI();
	glm::mat4 GetCamMat4();
	void OnRender();
	void OnInit();
	std::shared_ptr<Shader> _shader;
	std::vector<std::unique_ptr<Texture>> _textures;

};

