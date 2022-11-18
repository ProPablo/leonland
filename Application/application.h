#pragma once
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
namespace kongroo
{

	class Application
	{
	public:
		Application();
		void Run();
	private:

		GLFWwindow* _window = nullptr;
		ImVec4 _clear_color = {};
		void OnDrawUI();
		void OnRender();

	};
}

