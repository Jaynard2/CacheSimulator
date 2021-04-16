#version 430
layout (location = 0) in vec2 vert;

uniform mat4 mat;
uniform mat4 translationMat;

void main()
{
	gl_Position = mat * (translationMat * vec4(vert, 0.1, 1.0));
}
