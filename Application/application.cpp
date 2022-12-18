
#include <glad/glad.h> //has to be included before glfw
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "stb/stb.h"


#include "Application.h"
#include "rendering/Shader.h"
#include "rendering/Quad.h"

#include <iostream>
#include <string>
static void glfw_error_callback(int error, const char* description)
{
    //fprintf freaks out the compiler before init, so use cpp api 
    //fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    //const std::string s = std::to_string(error);
    std::cout << description << " :" << error << std::endl;
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
    _window = glfwCreateWindow(1280, 720, "Leon Land", NULL, NULL);
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
        ImGui::DragFloat2("Cam Position", glm::value_ptr(camRect.Pos));
        ImGui::DragFloat2("Cam Scale", glm::value_ptr(camRect.Bounds));

        ImGui::InputTextWithHint("Image file", "enter file loc here", imageFile, 255);
        if (ImGui::Button("Change Picture"))
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, GenerateTexture(imageFile));
        }
        ImGui::SameLine();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        if (wireFrame)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        _shader.SetUniformv4("_Color", squareColor);

        //transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0, 0, 1));
        //transform = glm::scale(transform, scale);


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
    glUseProgram(_shader);
    glBindVertexArray(_VAO);

    static Quad quads[] = {
        {{0,0}, {1,1}, 0.0},
        {{0.7,0}, {0.6,1}, 0.0}

    };

    auto camMat = GetCamMat4();
    _shader.SetMat4("cam", camMat);

    for (auto q : quads)
    {
        //Perform transformations
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(q.Pos, 0));
        transform = glm::rotate(transform, glm::radians(q.Angle), glm::vec3(0, 0, 1));
        transform = glm::scale(transform, glm::vec3(q.Scale, 1));
        _shader.SetMat4("transform", transform);

        //Last arguement is how many vertices we want to draw (this states were only going to draw using indices)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


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
        //positions		//color			 //text_coord	
         0.5,  0.5, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f,1.0f,			//top right,
         0.5, -0.5, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f,0.0f,	//bottom right
        -0.5, -0.5, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f,0.0f,	//bottom left
        -0.5,  0.5, 0.0f,	0.0f, 1.0f, 1.0f,	0.0f,1.0f,	//top left
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

    camRect.Bounds = { 1,1 };

    //OpenGl by default flips textures
    stbi_set_flip_vertically_on_load(true);

    //This stores the settings that glEnableVertattrib array and glVertexattrbPointer make
    //In future multiple buffers, layout groups and different shaders to use with those different layout groups will be used.
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);

    DrawQuad();

    Shader::Create(_shader, "shaders/vertex.glsl", "shaders/frag.glsl");
    glUseProgram(_shader);

    //(s,t,r) correspond to (x,y,z)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GenerateTexture("assets/container.jpg"));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, GenerateTexture("assets/agiri_chrismas.jpg"));
    _shader.SetUniformi("background", 0);
    _shader.SetUniformi("image", 1);


    //This uses the VBO currently bound using glBindBuffer
    //First arg is vertex attribute (layout (location = 0)) in vert shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attrib									//stride(space between each entry) //offset
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

}


GLuint Application::GenerateTexture(const std::string& filePath)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        log_error("Failed to load texture");
        return 0;
    }

    //https://stackoverflow.com/questions/71284184/opengl-distorted-texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //This is how bitpacking works with unsinged bytes (255) instead of full floats, doesnt make things slow excpet texture loading
    //https://stackoverflow.com/questions/11042027/glpixelstoreigl-unpack-alignment-1-disadvantages
    //This suggests that when changing etxtures to and from gpu, try to make it multiple of 8
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //2nd arg: mipmap level
    //3rd arg: texture type
    //7.8th format and datatype of input
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return texture;
}

glm::mat4 Application::GetCamMat4()
{
    auto camArr = camRect.GetBounds();
    //Very important that the clipping planes are properly adjusted or the 2d drawings that have a z position of 0 will not be drawn
    return glm::ortho(camArr[0], camArr[1], camArr[2], camArr[3], -1.0f, 1.0f);
}
