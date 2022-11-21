#version 330 core
//we  can use glGetAttribLocation instead of specifying a location by layout(location = 0)
layout(location = 0) in vec3 aPos;
//in and out keywords match with the piepline of how data is transfered between shaders

//In future we may have a colour tint in vec4
void main() 
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}