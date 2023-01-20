#version 330 core
//we  can use glGetAttribLocation instead of specifying a location by layout(location = 0)
//in and out keywords match with the piepline of how data is transfered between shaders
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aTexIndex;

//These interpolate from here to the fragment shader
out vec4 tintColor;
out vec2 TexCoord;
out float texIndex;

uniform mat4 cam;


void main() 
{
	//Often placd with projection * view * model * aPos
	gl_Position = cam * vec4(aPos, 1.0);
	tintColor = aColor;
	TexCoord = aTexCoord;
	texIndex = aTexIndex;
}