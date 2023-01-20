#include "Renderer.h"
#include "../core/Log.h"
#include "../core/Util.h"


//In order to debug array as pointer use:
//*(int(*)[10])some_pointer
// https://github.com/Microsoft/vscode-cpptools/issues/172

Renderer::Renderer()
{
    _vertexBuffer = new Vertex[MAX_VERTICES];
    _currentVertexPtr = _vertexBuffer;
    for (int i = 0; i < _textureSlots.size(); i++)
        _textureSlots[i] = nullptr;
}

Renderer::~Renderer()
{
    delete[] _vertexBuffer;
}

//Init is better than the ctor as it can be run at a certain point after stack alloced memory
void Renderer::Init(std::shared_ptr<Shader> shader)
{
    _shader = shader;
    //Initialize Vector data as an array
    //This stores the settings that glEnableVertattrib array and glVertexattrbPointer make
    //In future multiple buffers, layout groups and different shaders to use with those different layout groups will be used.
    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);

    //Vertex Buffer Object
    glGenBuffers(1, &_VBO);

    //We have to bind buffer every time we are going to use the buffer
    //The GL_ARRAY_BUFFER is there to indicate that this is to hold vertices
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_VERTICES, nullptr, GL_DYNAMIC_DRAW);
    auto floatSize = sizeof(float);
    auto res = member_size(Vertex, pos);

    //This uses the VBO currently bound using glBindBuffer
    //First arg is vertex attribute (layout (location = 0)) in vert shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

    //color attrib									//stride(space between each entry) //offset
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texIndex));

    GenerateEBO();


    //(s,t,r) correspond to (x,y,z)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int32_t samplers[MAX_ENVIRONMENT_TEXTURES];
    for (int32_t i = 0; i < MAX_ENVIRONMENT_TEXTURES; i++)
    {
        samplers[i] = i;
    }
    _shader->SetUniformiv("textures", samplers, MAX_ENVIRONMENT_TEXTURES);

    //Gets value inside the pointer and type converts it to gluint using conversion operator
    glUseProgram(*_shader);

}


void Renderer::AddQuad(const Quad& quad)
{
    auto currentSlot = _currentAvailTexSlot++;
    _textureSlots[currentSlot] = quad.Tex;
    //Add quad to vector
    auto newVecs = quad.ToVerts(currentSlot);
    for (auto& vec : newVecs)
    {
        *_currentVertexPtr = vec;
        _currentVertexPtr++;
    }
}

void Renderer::BeginBatch()
{
    _currentVertexPtr = _vertexBuffer;
    _currentAvailTexSlot = 0;
}

void Renderer::EndBatch()
{

    GLsizeiptr currentVertexCount = _currentVertexPtr - _vertexBuffer;
    auto totalQuads = currentVertexCount / 4;

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, currentVertexCount * sizeof(Vertex), _vertexBuffer);

    //Draw all elements (Flush)
    glUseProgram(*_shader);
    glBindVertexArray(_VAO);

    for (int i = 0; i < _currentAvailTexSlot; i++)
    {
        _textureSlots[i]->Bind(i);
    }

    glDrawElements(GL_TRIANGLES, totalQuads * 6, GL_UNSIGNED_INT, nullptr);

}


void Renderer::GenerateEBO()
{
    //art done in https://asciiflow.com/#/share/eJy10ksKwjAQANCrDLNSSI0fFNqtrrtyGZDQBgmkCaRRWou38DiexpOYUsFPSylIh5DMwGPChFSoeSYw0ielCCpeCosRVgwLhlG4WRGGpc%2BWYegzJwrnC4Z7K7k%2BKgGJ0bmzp8RJo0FqiHdbmMTGZlzJSy5SSMVZJrUzNpWaO5FDsABnYDFlTD9u97FXcwv0R6ebk%2FlsPcABZT4GtaQAHbaL0npr2w5Km6Nl25QeXvFr3zTwI5PgY%2B6v%2Bq%2Fn7HOjfwK84vUJ68El2w%3D%3D)
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

    uint32_t indicesLocalBuffer[MAX_INDICES];
    //uint32_t *indicesLocalBuffer = new uint32_t[MAX_INDICES];
    int offset = 0;
    for (int i = 0; i < MAX_INDICES; i += 6, offset += 4)
    {
        for (int j = 0; j < 6; j++)
            indicesLocalBuffer[i + j] = indices[j] + offset;
    }

    //Element Buffer Object
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MAX_INDICES, indicesLocalBuffer, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLocalBuffer), indicesLocalBuffer, GL_STATIC_DRAW);


}

