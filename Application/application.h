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

class Application
{
public:
	GLuint GenerateTexture(const std::string& filePath);
	void Run();
private:

	GLFWwindow* _window = nullptr;
	ImVec4 _clear_color = {};
	//orthographic projection will make it so z doesnt matter here
	//Currently, all positions go from -1 to 1
	glm::vec3 camPosition = {0.0f, 0.0f, 3.0f};
	Shader _shader;
	//GLuint _shaderProgram;
	//Vertex array object
	GLuint _VAO;
	void OnDrawUI();
	glm::mat4 GetCamMat4();
	void OnRender();
	void OnInit();

};

