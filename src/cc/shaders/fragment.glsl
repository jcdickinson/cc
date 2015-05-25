#version 330 core

in vec2 UV;
out vec4 color;
uniform sampler2D myTextureSampler;

void main()
{
	vec2 uv = UV;
	uv.y = 1 - uv.y;
	color = texture(myTextureSampler, uv).rgba;
}
