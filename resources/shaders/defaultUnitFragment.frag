#version 330 core

layout (location = 0) out vec4 color;

in vec2 texCoord;
uniform sampler2D tex0;
uniform bool selected;
uniform bool isMainPlayer;

void main()
{
	vec4 main = texture(tex0, texCoord);
	if (isMainPlayer == true)
	{
		color = main;
	}
	else
	{
		color = main * vec4(0.3f, 0.0f, 0.0f, 1.0f);
	}
}