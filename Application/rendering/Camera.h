#pragma once
#include "Quad.h"
class Camera
{
private:

public:
	//This will show how long the dominant axis will show
	float size = 1;
	Rect camRect = { {0,0}, {5.0,5.0} };
	glm::mat4 GetMat4();
};

