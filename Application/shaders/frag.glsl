#version 330 core
out vec4 FragColor;
uniform vec4 _Color;
uniform sampler2D background;
uniform sampler2D image;
uniform sampler2D textures[32];

in vec4 tintColor;
in vec2 TexCoord;
in float texIndex;

void main() 
{
	int index = int(texIndex);
	//FragColor = vec4(texIndex, texIndex,texIndex, 1);
	//FragColor = _Color;
	//FragColor = texture(image, TexCoord) * tintColor;
	FragColor = vec4(TexCoord.x,TexCoord.y, 0, 1);
	//vec4 backgroundColor = texture(background, TexCoord);
	//'FragColor = texture(textures[1], TexCoord);
	//FragColor = mix(texture(image, TexCoord), backgroundColor, 0.2);
}