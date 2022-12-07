#version 330 core
//we  can use glGetAttribLocation instead of specifying a location by layout(location = 0)
//in and out keywords match with the piepline of how data is transfered between shaders
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

//These interpolate from here to the fragment shader
out vec3 tintColor;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 cam;


void main() 
{
	gl_Position = transform * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos, 1.0);
	tintColor = aColor;
	TexCoord = aTexCoord;
}