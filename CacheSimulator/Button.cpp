#include "Button.h"
#include <gl/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include <GLFW/glfw3.h>

GLFWcursor* Button::s_pointer = nullptr;
GLFWcursor* Button::s_hand = nullptr;
bool Button::s_cursorSet = false;

Button::Button(std::shared_ptr<GLRenderingProgram> txtProg, std::shared_ptr<GLRenderingProgram> btnProg, 
	float width, float height, std::string label, glm::vec2 pos)
	: RenderObject(btnProg), _width(width), _height(height), _label(txtProg, label), _pos(pos)
{
}

Button::Button(std::shared_ptr<GLRenderingProgram> prog, float width, float height, 
	RenderString label, glm::vec2 pos)
	: RenderObject(prog), _width(width), _height(height), _label(label), _pos(pos)
{
}

void Button::draw(const glm::mat4& matrix)
{
	RenderObject::draw(matrix);

	glFrontFace(GL_CW);

	glBindBuffer(GL_ARRAY_BUFFER, p_vbos[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(_pos.x, _pos.y, 0.0f));

	glUniformMatrix4fv(p_prog->getUniformLoc("mat"), 1, GL_FALSE, glm::value_ptr(matrix));
	glUniformMatrix4fv(p_prog->getUniformLoc("translationMat"), 1, GL_FALSE, glm::value_ptr(translation));
	glUniform3f(p_prog->getUniformLoc("color"), _color.r, _color.g, _color.b);


	glDrawArrays(GL_TRIANGLES, 0, p_verts.size() / 2);

	_label.draw(matrix);
	Button::s_cursorSet = false;
	
}

void Button::setup()
{
	p_verts = {
		0.0f, _height,
		_width, _height,
		0.0f, 0.0f,

		0.0f, 0.0f,
		_width, _height,
		_width, 0.0f
	};

	p_vbos.push_back(GLuint());
	glGenBuffers(1, &p_vbos[0]);

	glBindBuffer(GL_ARRAY_BUFFER, p_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p_verts[0]) * p_verts.size(), &p_verts[0], GL_STATIC_DRAW);

	_label.setPosition(_pos + glm::vec2(_width / 4.0f - _label.getWidth() / 2.0f,
		_height / 2.0f - Character::getAdvanceY() / 2.0f));

	_color = glm::vec3(0.8f, 0.8f, 0.8f);

	if (Button::s_pointer == nullptr)
	{
		Button::s_pointer = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		Button::s_hand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	}

	_label.setColor(glm::vec3(0.0f, 0.0f, 0.0f));
	
}

void Button::setCallbackFunc(std::function<void()> func)
{
	_callback = func;
}

void Button::cursorMoveCallback(GLFWwindow* window, double x, double y)
{
	if (x > _pos.x && x < _pos.x + _width)
	{
		if (720 - y < _pos.y + _height && 720 - y > _pos.y)
		{
			_color = glm::vec3(0.4f, 0.4f, 0.4f);
			Button::s_cursorSet = true;
			glfwSetCursor(window, Button::s_hand);
			return;
		}
	}

	_color = glm::vec3(0.8f, 0.8f, 0.8f);

	if(!Button::s_cursorSet)
		glfwSetCursor(window, Button::s_pointer);
	
}

void Button::mouseCallback(GLFWwindow* win, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(win, &x, &y);

	if (x > _pos.x && x < _pos.x + _width)
	{
		if (720 - y < _pos.y + _height && 720 - y > _pos.y)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
				_callback();
		}
	}
	
}

