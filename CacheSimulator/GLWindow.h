#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <map>

#include "RenderObject.h"
#include "TextBox.h"
#include "Button.h"
#include "OpenFile.h"
#include "SaveFileController.h"

constexpr unsigned SET_NUM = 4;
constexpr unsigned FIELDS = 5;

class GLWindow
{
public:
	GLWindow(std::string title, unsigned width, unsigned height);
	~GLWindow();

	void start();


private:
	unsigned _windowWidth;
	unsigned _windowHeight;
	std::string _windowTitle;
	GLFWwindow* _window;

	std::vector<RenderObject*> _objs;

	glm::mat4 _mat;

	struct glfwPointers
	{
		TextBox* mainMem[2];
		TextBox* location;
		Button* file;
		Button* simulate;
	} _ptrs;

	OpenFileController* of;
	TextBox* holder;
	TextBox* _location;
	TextBox* _msg;
	TextBox* _RAMTextBoxes[SET_NUM][FIELDS];
	std::vector<std::string> _memory;
	bool _oldest[SET_NUM];
	glm::ivec2 _lastUsed;

	void onGLFWError(int error, const char* description);

};
