#pragma once
#include "Shader.h"
#include <glad/glad.h>
#include "Quad.h"

struct Vertex
{
	float pos[3];
	float color[4];
	float texcoord[4];
	float texID;
};

class Renderer
{
public:
	static const int MAX_VERTICES = 10000;
	Renderer(std::shared_ptr<Shader> shader);
	void Init();
	void AddQuad(Quad quad);
	void BeginBatch();
	void EndBatch();
	void Flush();
	GLuint GenerateTexture(const std::string& filePath);
	//Shader Shader;

private:
	
	GLuint _VAO;
	int _currentVertex;
	Vertex _vertices[MAX_VERTICES];
	std::shared_ptr<Shader> _shader;
};

