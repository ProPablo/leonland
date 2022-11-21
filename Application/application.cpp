
#include <glad/glad.h> //has to be included before glfw
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "Application.h"
#include "rendering/Shader.h"

#include <iostream>
#include <string>
static void glfw_error_callback(int error, const char* description)
{
	//fprintf freaks out the compiler before init, so use cpp api 
	//fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	//const std::string s = std::to_string(error);
	std::cout << description << " :" << error << std::endl;
}

//static void log_error(const std::string& errorString)
//{
//	std::cout << errorString << std::endl;
//}

Application::Application()
{
	//Init imgui style here

}

void Application::Run()
{
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
	_window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if (_window == NULL)
		log_error("No _window");

	glfwMakeContextCurrent(_window);

	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSwapInterval(1); // Enable vsync


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
		// Poll and handle events (inputs, _window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();



		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		OnDrawUI();

		// Rendering
		ImGui::Render();
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

void Application::OnDrawUI()
{

	// 1. Show the big demo _window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (show_demo__window)
	ImGui::ShowDemoWindow();

	// 2. Show a simple _window that we create ourselves. We use a Begin/End pair to created a named _window.
	{
		static float f = 0.0f;
		static int counter = 0;
		static bool wireFrame = false;

		ImGui::Begin("Hello, world!");                          // Create a _window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Wire Frame", &wireFrame);      // Edit bools storing our _window open/close state
		//ImGui::Checkbox("Another Window", &show_another__window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&_clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		if (wireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

}

void Application::OnRender()
{
	//Clear buffer
	int display_w, display_h;
	//Can use set framebuffer size callback instyead
	glfwGetFramebufferSize(_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(_clear_color.x * _clear_color.w, _clear_color.y * _clear_color.w, _clear_color.z * _clear_color.w, _clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw all elements
	glUseProgram(_shaderProgram);
	glBindVertexArray(_VAO);
	//Last arguement is how many vertices we want to draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


}

void DrawQuad()
{
	//art done in https://asciiflow.com/#/share/eJy10ksKwjAQANCrDLNSSI0fFNqtrrtyGZDQBgmkCaRRWou38DiexpOYUsFPSylIh5DMwGPChFSoeSYw0ielCCpeCosRVgwLhlG4WRGGpc%2BWYegzJwrnC4Z7K7k%2BKgGJ0bmzp8RJo0FqiHdbmMTGZlzJSy5SSMVZJrUzNpWaO5FDsABnYDFlTD9u97FXcwv0R6ebk%2FlsPcABZT4GtaQAHbaL0npr2w5Km6Nl25QeXvFr3zTwI5PgY%2B6v%2Bq%2Fn7HOjfwK84vUJ68El2w%3D%3D)
	/*
			Triangle construction in NDC (Normalizsed device coordinates -1 to 1)
			+-------------------------------+
			|                               |
			|            0,0.5              |
			|             /\                |
			|            /  \               |
			|           /    \              |
			|          /      \             |
			|         /________\            |
			| -0.5,-0.5        0.5,-0.5     |
			|                               |
			|                               |
			+-------------------------------+
	*/

	float vertices[] = {
		0.5, 0.5, 0, //top right
		0.5, -0.5, 0, //bottom right
		-0.5, -0.5, 0, //bottom left
		-0.5,  0.5, 0 //top left
	};
	//Vertex Buffer Object
	GLuint VBO;
	glGenBuffers(1, &VBO);

	//We have to bind buffer every time we are going to use the buffer
	//The GL_ARRAY_BUFFER is there to indicate that this is to hold vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
		Indices and which vertices are being used
		-----------------------
			  3         0
			   +-------+
			   |\      |
			   | \     |
			   |  \ 1  |
			   |   \   |
			   |    \  |
			   |  2  \ |
			   |      \|
			   +-------+
			  2         1

		-----------------------
	*/

	int indices[] = {
		0,1,3,
		1,2,3
	};

	//Element Buffer Object
	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Application::OnInit()
{
	//This stores the settings that glEnableVertattrib array and glVertexattrbPointer make
	//In future multiple buffers, layout groups and different shaders to use with those different layout groups will be used.
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	DrawQuad();
	bool didShaderSucceed;
	Shader vertexShader("shaders/vertex.glsl", GL_VERTEX_SHADER, didShaderSucceed);
	if (!didShaderSucceed) return;
	Shader fragShader("shaders/frag.glsl", GL_FRAGMENT_SHADER, didShaderSucceed);
	if (!didShaderSucceed) return;
	_shaderProgram = Shader::CompileProgram(vertexShader, fragShader);

	//Only need program ref to upload uniforms and do anything
	glDeleteShader(vertexShader._shaderRef);
	glDeleteShader(fragShader._shaderRef);

	//This uses the VBO currently bound using glBindBuffer
	//First arg is vertex attribute (layout (location = 0)) in vert shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glUseProgram(_shaderProgram);
}
