#version 430
layout (location = 0) in vec2 pos;
uniform mat4 mat;


void main()
{
	gl_Position = mat * vec4(pos.x, pos.y, 0.2f, 1.0f);
}
