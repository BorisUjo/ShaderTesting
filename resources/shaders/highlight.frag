#version 330 core

layout (location = 0) out vec4 color;

in vec2 texCoords;

uniform sampler2D tex0;
uniform bool selected;
void main()
{
	
	if (!selected)
	{
		color = texture(tex0, texCoords);
	}
	else
	{
		color = vec4(0.2f, 0.2f, 0.7f, 1.0f) * texture(tex0, texCoords);
	}
	//color = vec4(texCoords, 0,1);

}