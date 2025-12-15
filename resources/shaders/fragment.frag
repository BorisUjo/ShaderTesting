#version 330 core

layout (location = 0) out vec4 color;

in vec2 texCoord;
uniform sampler2D tex0;
uniform bool selected;
uniform bool DebugSelect;

void main()
{

	if (DebugSelect == true)
	{
		color = texture(tex0, texCoord) * vec4(0.0f, 0.3f, 1.0f, 1.0f);
		return;
	}

	if (selected == false)
	{
		color = texture(tex0, texCoord);
	}
	else
	{
		color = texture(tex0, texCoord) * vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
}