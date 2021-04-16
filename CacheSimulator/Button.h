#pragma once
#include "RenderObject.h"
#include <functional>
#include <glm/common.hpp>
#include <memory>
#include <GLFW/glfw3.h>

#include "GLRenderingProgram.h"
#include "RenderString.h"

class Button : public RenderObject
{
public:
	Button(std::shared_ptr<GLRenderingProgram> txtProg, std::shared_ptr<GLRenderingProgram> btnProg, 
		float width, float height, std::string label, glm::vec2 pos);
	Button(std::shared_ptr<GLRenderingProgram> prog, float width, float height, RenderString label, glm::vec2 pos);

	void draw(const glm::mat4& matrix);
	void setup();
	void update(double time) {};

	void setLabel(RenderString label) { _label = label; }

	void setCallbackFunc(std::function<void()> func);

	void cursorMoveCallback(GLFWwindow* window, double x, double y);
	void mouseCallback(GLFWwindow* win, int button, int action, int mods);

private:
	float _width;
	float _height;
	glm::vec2 _pos;
	glm::vec3 _color;

	RenderString _label;
	std::function<void()> _callback;

	static GLFWcursor* s_pointer;
	static GLFWcursor* s_hand;
	static bool s_cursorSet;

};
