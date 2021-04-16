#version 430
layout (location = 0) in vec4 vertex;
out vec2 texCoords;

uniform mat4 proj;
uniform float texOffset;
uniform mat4 translationMat;

void main()
{
	gl_Position =  proj * (translationMat * vec4(vertex.x, vertex.y, 0.1, 1.0));
	texCoords = vec2(vertex.z + texOffset, vertex.w);
}
