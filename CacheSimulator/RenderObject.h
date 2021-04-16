#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "GLRenderingProgram.h"

class RenderObject
{
public:
	RenderObject(std::shared_ptr<GLRenderingProgram> program);

	virtual void draw(const glm::mat4& matrix);
	virtual void update(double time) { /*Does Nothing*/ }
	virtual void setup() = 0;

	//Getters
	const std::shared_ptr<GLRenderingProgram>& getProgram() const { return p_prog; }
	const std::vector<GLuint>& getVBOs() const { return p_vbos; }

protected:
	std::shared_ptr<GLRenderingProgram> p_prog;

	GLuint p_vao;
	std::vector<GLuint> p_vbos;
	std::vector<float> p_verts;
};

