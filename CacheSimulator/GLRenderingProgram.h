#pragma once
#include <string>
#include <GL/glew.h>
#include <memory>

#include "GLShader.h"

class GLRenderingProgram
{
public:
	GLRenderingProgram(const std::string& vertexShader, const std::string& fragShader);
	GLRenderingProgram(std::shared_ptr<GLShader> vShader, std::shared_ptr<GLShader> fShader);

	void use() const;

	std::shared_ptr<GLShader> getVertexShader() const { return _vShader; }
	std::shared_ptr<GLShader> getFragmentShader() const { return _fShader; }
	const GLint& getID() const { return _prog; }
	const GLint& getUniformLoc(std::string uniformName) const;

private:
	std::shared_ptr<GLShader> _vShader;
	std::shared_ptr<GLShader> _fShader;
	GLint _prog;

	std::string _errLog;

	void init();
	void checkOpenGLError();
	void saveProgramLog();

};
