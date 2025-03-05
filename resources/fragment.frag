#version 330 core

layout (location = 0) out vec4 color;

in vec2 texCoords;

uniform sampler2D tex0;

void main()
{
	
	color = texture(tex0, texCoords);
	//color = vec4(texCoords, 0,1);

}