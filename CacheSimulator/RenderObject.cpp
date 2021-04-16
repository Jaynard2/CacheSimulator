#include "RenderObject.h"

RenderObject::RenderObject(std::shared_ptr<GLRenderingProgram> program) : p_prog(program)
{
	glGenVertexArrays(1, &p_vao);
}

/*
* Children should call this at start of their own draw call 
*/
void RenderObject::draw(const glm::mat4& matrix)
{
	p_prog->use();
	glBindVertexArray(p_vao);
}
