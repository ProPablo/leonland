#version 330 core
out vec4 FragColor;
uniform vec4 _Color;
uniform sampler2D background;
uniform sampler2D image;

in vec3 tintColor;
in vec2 TexCoord;

void main() 
{
	//FragColor = _Color;
	//FragColor = texture(image, TexCoord) * vec4(tintColor, 1.0);
	vec4 backgroundColor = texture(background, TexCoord);
	//FragColor = texture(image, TexCoord);
	FragColor = mix(texture(image, TexCoord), backgroundColor, 0.2);
}