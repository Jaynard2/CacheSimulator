#pragma once
#include "RenderString.h"
#include <glm/common.hpp>
#include <memory>
#include <GLFW/glfw3.h>

#include "GLRenderingProgram.h"

class TextBox : public RenderObject
{
public:
	TextBox(std::shared_ptr<GLRenderingProgram> prog, std::shared_ptr<GLRenderingProgram> textBox, glm::vec2 topLeft, glm::vec2 bottomRight);

	void draw(const glm::mat4& matrix) override;
	void setup() override;

	std::string getString() const { return _str.c_str(); }
	void setString(const std::string& str) { _str = str; }
	void setEnabled(bool en);
	void setTextColor(const glm::vec3& color) { _textColor = color; }
	void setBackgroundColor(const glm::vec4& backgroundColor) { _backColor = backgroundColor; }
	void setCentered(bool center) { _centered = center; }

	void charCallback(GLFWwindow* win, unsigned int codepoint);
	void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods);
	void scrollCallback(GLFWwindow* win, double x, double y);

private:
	glm::vec2 _left;
	glm::vec2 _right;
	glm::vec3 _textColor;
	glm::vec4 _backColor;
	float _scrolled;
	bool _centered;

	bool _enabled;

	RenderString _str;

};

