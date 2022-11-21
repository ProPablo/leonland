#version 330 core
out vec4 FragColor;
uniform vec4 _Color;

void main() 
{
	//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	FragColor = _Color;
}