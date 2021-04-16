#version 430
in vec2 texCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, vec2(texCoords.x, texCoords.y)).r);
	color = vec4(textColor, 1.0) * sampled;
}
