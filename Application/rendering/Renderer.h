#pragma once
#include "Shader.h"
#include <glad/glad.h>
#include "Quad.h"
#include "Vertex.h"
#include "Texture.h"


class Renderer
{
public:
    Renderer();
    ~Renderer();

    static const size_t MAX_VERTICES = 10000;
    static const size_t MAX_INDICES = (MAX_VERTICES / 4) * 6;
    static const size_t MAX_ENVIRONMENT_TEXTURES = 32;
    void Init(std::shared_ptr<Shader> shader);
    void AddQuad(const Quad& quad);
    void BeginBatch();
    void EndBatch();
    //void Flush();

private:

    GLuint _VAO = -1;
    GLuint _VBO = -1;
    //int _currentVertexCount;
    Vertex* _vertexBuffer;
    Vertex* _currentVertexPtr;
    //Declaring verts like this hsa the possibility of stack overflow, for large buffers of data, its better to declare on the heap
    //Vertex _vertices[MAX_VERTICES];
    std::shared_ptr<Shader> _shader = nullptr;
    //The texture slots correspond to the index and assigning a TextureInstance binds the texture to the slot
    std::array<Texture*, MAX_ENVIRONMENT_TEXTURES> _textureSlots;
    int _currentAvailTexSlot = 0;

    // Doesnt work because Texture is a class that has to be run through its ctor
    //std::array<Texture, MAX_ENVIRONMENT_TEXTURES> _textureSlots;

    void GenerateEBO();
};

