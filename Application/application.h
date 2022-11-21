#pragma once
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "rendering/Shader.h"

//TODO: make precompiled header with string and iostream if using this

//https://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#if _WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif
#define log_error(input) std::cout << __FILENAME__ << " (Error): " << input << std::endl;
#define log_dbg(input) std::cout << __FILENAME__ << " (Debug): " << input << std::endl;

//static void log_error(const std::string& errorString);

class Application
{
public:
	void Run();
private:

	GLFWwindow* _window = nullptr;
	ImVec4 _clear_color = {};
	Shader _shader;
	//GLuint _shaderProgram;
	//Vertex array object
	GLuint _VAO;
	void OnDrawUI();
	void OnRender();
	void OnInit();

};

