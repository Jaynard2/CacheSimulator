#include "TextBox.h"
#include <glm/gtc/type_ptr.hpp>

TextBox::TextBox(std::shared_ptr<GLRenderingProgram> prog, std::shared_ptr<GLRenderingProgram> textBox, glm::vec2 topLeft, glm::vec2 bottomRight) : _left(topLeft), _right(bottomRight), RenderObject(textBox), _enabled(true), _str(prog), _scrolled(0.0f)
{
	_str.setPosition(_left - glm::vec2(0.0f, _str.getFontSize()));
	_textColor = glm::vec3(1.0f, 1.0f, 1.0f);
	_centered = false;
	_backColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	p_verts = {
		_left.x, _left.y,
		_left.x, _right.y,
		_right.x, _left.y,

		_right.x, _left.y,
		_left.x, _right.y,
		_right.x, _right.x
	};
}

void TextBox::draw(const glm::mat4 & matrix)
{
	if (_enabled)
	{
		RenderObject::draw(matrix);
		
		p_verts = {
			_left.x, _left.y,
			_left.x, _right.y,
			_right.x, _left.y,

			_right.x, _left.y,
			_left.x, _right.y,
			_right.x, _right.y
		};

		glBindBuffer(GL_ARRAY_BUFFER, p_vbos[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, p_verts.size() * sizeof(p_verts[0]), &p_verts[0]);
		
		glUniform4fv(p_prog->getUniformLoc("backgroundColor"), 1, glm::value_ptr(_backColor));
		glUniformMatrix4fv(p_prog->getUniformLoc("mat"), 1, GL_FALSE, glm::value_ptr(matrix));

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		glFrontFace(GL_CCW);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glm::vec2 pos = _left - glm::vec2(0.0f, _str.getFontSize());

		if (_centered)
			pos += glm::vec2((_right.x - _left.x - _str.getWidth()) / 2.0f, 0.0f);

		_str.setPosition(pos);
		_str.setColor(_textColor);
		_str.draw(matrix);
	}
}

void TextBox::setup()
{
	glBindVertexArray(p_vao);

	p_vbos.push_back(GLuint());
	glGenBuffers(1, &p_vbos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, p_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, p_verts.size() * sizeof(p_verts[0]), &p_verts[0], GL_DYNAMIC_DRAW);
}

void TextBox::setEnabled(bool en)
{
	_enabled = en;

	if (_enabled)
	{
		_left.y += _scrolled;
		_scrolled = 0.0f;
	}
}

void TextBox::charCallback(GLFWwindow * win, unsigned int codepoint)
{
	if (_str.getWidth() < _right.x - _str.getFontSize() && _enabled)
		_str.push_back((char)codepoint);
}

void TextBox::keyCallback(GLFWwindow * win, int key, int scancode, int action, int mods)
{
	if (_enabled)
	{
		if (key == GLFW_KEY_ENTER)
		{
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				_str.push_back('\n');
			}
		}
		else if (key == GLFW_KEY_BACKSPACE && !_str.empty())
		{
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				_str.pop_back();
			}
		}
	}
}

void TextBox::scrollCallback(GLFWwindow * win, double x, double y)
{
	if (_enabled)
	{
		float scrollAmount = y * Character::getAdvanceY() * 2;
		_left.y -= scrollAmount;
		_scrolled += scrollAmount;
	}
}
