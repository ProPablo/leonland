#include "Renderer.h"
#include "../stb/stb.h"
#include "../application.h"

//Init is better than the ctor as it can be run at a certain point after stack alloced memory
void Renderer::Init()
{
	//Initialize Vector data as an array
    //This stores the settings that glEnableVertattrib array and glVertexattrbPointer make
    //In future multiple buffers, layout groups and different shaders to use with those different layout groups will be used.
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);

    //Vertex Buffer Object
    GLuint VBO;
    glGenBuffers(1, &VBO);

    //We have to bind buffer every time we are going to use the buffer
    //The GL_ARRAY_BUFFER is there to indicate that this is to hold vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_DYNAMIC_DRAW);

    int indices[] = {
        0,1,3,
        1,2,3
    };

    //Element Buffer Object
    GLuint EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //This uses the VBO currently bound using glBindBuffer
    //First arg is vertex attribute (layout (location = 0)) in vert shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attrib									//stride(space between each entry) //offset
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    Shader::Create(Shader, "shaders/vertex.glsl", "shaders/frag.glsl");
    glUseProgram(Shader);

    //(s,t,r) correspond to (x,y,z)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE0);

    //OpenGl by default flips textures (uv starts at bottom left and the image is loaded from top left)
    stbi_set_flip_vertically_on_load(true);


}


void Renderer::AddQuad(Quad quad)
{
    //Add quad to vector

    //Setup 
}

void Renderer::BeginBatch()
{
    _currentVertex = 0;
    //Clear all vertices

}

void Renderer::EndBatch()
{
}

void Renderer::Flush()
{
}



GLuint Renderer::GenerateTexture(const std::string& filePath)
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

