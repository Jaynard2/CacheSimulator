#include "GLWindow.h"
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <Windows.h>

#undef _DEBUG

#include "OpenGLFailure.h"
#include "Character.h"
#include "RenderString.h"
#include "Button.h"
#include "Debug.h"

GLWindow::GLWindow(std::string title, unsigned width, unsigned height) : _windowWidth(width), _windowHeight(height), _windowTitle(title), _window(nullptr)
{
#ifndef _DEBUG
	FreeConsole();
#endif // _DEBUG

	if (!glfwInit())
		throw OpenGLFailure("Failed to initialize glfw");

	glfwSetErrorCallback((GLFWerrorfun)&[this](int error, const char* description) { onGLFWError(error, description); });

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef _DEBUG

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#endif // _DEBUG

	_window = glfwCreateWindow(_windowWidth, _windowHeight, _windowTitle.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(_window);

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
		throw OpenGLFailure((char*)glewGetErrorString(glewError));

#ifdef _DEBUG

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

#endif // _DEBUG


	glfwSwapInterval(1);

	glm::mat4 proj = glm::ortho(0.0f, (float)_windowWidth, 0.0f, (float)_windowHeight, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	_mat = proj * view;
	
	std::shared_ptr<GLRenderingProgram> characterProg = std::make_shared<GLRenderingProgram>("charVert.glsl", "charFrag.glsl");
	Character::generateLookupTable(24);

	std::shared_ptr<GLRenderingProgram> textboxProg = std::make_shared<GLRenderingProgram>("textboxVert.glsl", "textboxFrag.glsl");
	std::shared_ptr<GLRenderingProgram> buttonProg = std::make_shared<GLRenderingProgram>("btnVert.glsl", "btnFrag.glsl");

	glm::vec3 h1Title = glm::vec3(124 / 255.0f, 252 / 255.0f, 0.0f);
	glm::vec3 h2Title = glm::vec3(1.0f, 1.0f, 0.0f);

	TextBox* memTitle = new TextBox(characterProg, textboxProg, glm::vec2(_windowWidth / 2.0f, _windowHeight), glm::vec2(_windowWidth, _windowHeight - 30.0f));
	memTitle->setString("Main Memory");
	memTitle->setTextColor(h1Title);
	memTitle->setCentered(true);

	TextBox* mainAddrTitle = new TextBox(characterProg, textboxProg, glm::vec2(_windowWidth / 2.0f, _windowHeight - 30), glm::vec2(3.0f * _windowWidth / 4.0f, _windowHeight - 60.0f));
	TextBox* mainDataTitle = new TextBox(characterProg, textboxProg, glm::vec2(3.0f * _windowWidth / 4.0f, _windowHeight - 30), glm::vec2(_windowWidth, _windowHeight - 60.0f));
	mainAddrTitle->setString("Address");
	mainDataTitle->setString("Data");
	mainAddrTitle->setTextColor(h2Title);
	mainDataTitle->setTextColor(h2Title);
	mainAddrTitle->setCentered(true);
	mainDataTitle->setCentered(true);

	TextBox* mainAddr = new TextBox(characterProg, textboxProg, glm::vec2(_windowWidth / 2.0f, _windowHeight - 60), glm::vec2( 3.0f * _windowWidth / 4.0f, 0.0f));
	TextBox* mainData = new TextBox(characterProg, textboxProg, glm::vec2( 3 * _windowWidth / 4.0f, _windowHeight - 60), glm::vec2(_windowWidth, 0.0f));
	mainAddr->setCentered(true);
	mainData->setCentered(true);

	_ptrs.mainMem[0] = mainAddr;
	_ptrs.mainMem[1] = mainData;

	glfwSetScrollCallback(_window, [](GLFWwindow* window, double x, double y)
	{
		glfwPointers* ptrs = (glfwPointers*)glfwGetWindowUserPointer(window);
		ptrs->mainMem[0]->scrollCallback(window, x, y);
		ptrs->mainMem[1]->scrollCallback(window, x, y);
	});

	glm::vec2 RAMtop = glm::vec2(0.0f, _windowHeight / 2.0f);
	float lineHeight = -30.0f;

	TextBox* RAMtitle = new TextBox(characterProg, textboxProg, RAMtop, RAMtop + glm::vec2(_windowWidth / 2.0f, lineHeight));
	RAMtitle->setString("2-Way Set Associative RAM");
	RAMtitle->setCentered(true);
	RAMtitle->setTextColor(h1Title);

	TextBox* setTitle = new TextBox(characterProg, textboxProg, RAMtop + glm::vec2(0.0f, lineHeight), RAMtop + glm::vec2(_windowWidth / 10.0f, 2 * lineHeight));
	setTitle->setString("Set");
	setTitle->setTextColor(h2Title);
	setTitle->setCentered(true);

	TextBox* tagTitle1 = new TextBox(characterProg, textboxProg, RAMtop + glm::vec2(_windowWidth / 10.0f, lineHeight), RAMtop + glm::vec2(2 * _windowWidth / 10.0f, 2 * lineHeight));
	tagTitle1->setString("Tag");
	tagTitle1->setTextColor(h2Title);
	tagTitle1->setCentered(true);

	TextBox* dataTitle1 = new TextBox(characterProg, textboxProg, RAMtop + glm::vec2(2 * _windowWidth / 10.0f, lineHeight), RAMtop + glm::vec2(3 * _windowWidth / 10.0f, 2 * lineHeight));
	dataTitle1->setString("Data");
	dataTitle1->setTextColor(h2Title);
	dataTitle1->setCentered(true);

	TextBox* tagTitle2 = new TextBox(characterProg, textboxProg, RAMtop + glm::vec2(3 * _windowWidth / 10.0f, lineHeight), RAMtop + glm::vec2(4 * _windowWidth / 10.0f, 2 * lineHeight));
	tagTitle2->setString("Tag");
	tagTitle2->setTextColor(h2Title);
	tagTitle2->setCentered(true);

	TextBox* dataTitle2 = new TextBox(characterProg, textboxProg, RAMtop + glm::vec2(4 * _windowWidth / 10.0f, lineHeight), RAMtop + glm::vec2(5 * _windowWidth / 10.0f, 2 * lineHeight));
	dataTitle2->setString("Data");
	dataTitle2->setTextColor(h2Title);
	dataTitle2->setCentered(true);

	for (unsigned i = 0; i < SET_NUM; i++)
	{
		for (unsigned j = 0; j < FIELDS; j++)
		{
			TextBox* box = new TextBox(characterProg, textboxProg, RAMtop + glm::vec2(j * _windowWidth / 10.0f, (2 + i) * lineHeight), RAMtop + glm::vec2((1 + j) * _windowWidth / 10.0f, (3 + i) * lineHeight));
			box->setCentered(true);

			if (j == 0)
			{
				std::stringstream str;
				str << i;
				box->setString(str.str());
				box->setTextColor(glm::vec3(0.5f, 0.5f, 0.5f));
			}

			_RAMTextBoxes[i][j] = box;
			_objs.push_back(box);
		}
	}

	memset(_oldest, false, SET_NUM);

	holder = new TextBox(characterProg, textboxProg, { 0.0f, 0.0f }, { 0.0f, 0.0f });
	holder->setEnabled(false);

	of = new OpenFileController(holder);

	Button* loadFile = new Button(characterProg, buttonProg, _windowWidth / 5.0f, 40.0f, "Load File", glm::vec2(10.0f, _windowHeight - 55.0f));
	loadFile->setCallbackFunc([this]() 
	{
		of->operator()();
		std::string str = holder->getString();
		std::stringstream stream;
		stream << str;

		_memory.clear();

		std::string addr = "";
		std::string data = "";

		while (!stream.eof())
		{
			std::string line = "";
			stream >> line;

			auto delim = line.find(",");
			addr.append(line.substr(0, delim));
			addr.append("\n");
			data.append(line.substr(delim + 1, line.length() - delim));
			data.append("\n");

			_memory.push_back(line.substr(delim + 1, line.length() - delim));
		}

		_ptrs.mainMem[0]->setString(addr);
		_ptrs.mainMem[1]->setString(data);
	});
	_ptrs.file = loadFile;

	_location = new TextBox(characterProg, textboxProg, glm::vec2(10.0f, _windowHeight - 60.0f), glm::vec2(10.0f + _windowWidth / 5.0f, _windowHeight - 100.0f));
	_location->setBackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_location->setTextColor(glm::vec3(0.0f, 0.0f, 0.0f));
	_ptrs.location = _location;

	glfwSetCharCallback(_window, [](GLFWwindow* win, unsigned int codepoint)
	{
		glfwPointers* ptrs = (glfwPointers*)glfwGetWindowUserPointer(win);
		ptrs->location->charCallback(win, codepoint);;
	});

	glfwSetKeyCallback(_window, [](GLFWwindow* win, int key, int scancode, int action, int mods)
	{
		glfwPointers* ptrs = (glfwPointers*)glfwGetWindowUserPointer(win);
		ptrs->location->keyCallback(win, key, scancode, action, mods);
	});

	_msg = new TextBox(characterProg, textboxProg, glm::vec2(10.0f, _windowHeight - 105.0f), glm::vec2(10.0f + 2 * _windowHeight / 5.0f, _windowHeight - 145.0f));


	Button* sim = new Button(characterProg, buttonProg, _windowWidth / 5.0f, 40.0f, "Load Address", glm::vec2(20.0f + _windowWidth / 5.0f, _windowHeight - 100.0f));
	sim->setCallbackFunc([this]()
	{
		if (_memory.empty())
		{
			_msg->setString("Load a file into main memory");
			_msg->setTextColor(glm::vec3(1.0f, 0.2f, 0.5f));
			return;
		}

		if (_lastUsed.y != 0)
		{
			_RAMTextBoxes[_lastUsed.x][_lastUsed.y]->setTextColor(glm::vec3(1.0f, 1.0f, 1.0f));
			_RAMTextBoxes[_lastUsed.x][_lastUsed.y + 1]->setTextColor(glm::vec3(1.0f, 1.0f, 1.0f));
		}

		std::string memLocStr = _location->getString();

		bool isBinary = std::find_if_not(memLocStr.begin(), memLocStr.end(), [](auto c) {return c == '0' || c == '1'; }) == memLocStr.end();
		if (!isBinary || memLocStr.empty())
		{
			_msg->setString("Memory Address must be in binary");
			_msg->setTextColor(glm::vec3(1.0f, 0.2f, 0.5f));
			return;
		}

		int memLoc = strtol(memLocStr.c_str(), nullptr, 2);

		int set = memLoc % 4;

		std::string tagStr = "";
		
		if (memLoc < 4)
			tagStr = "0";
		else
			tagStr = memLocStr.substr(0, memLocStr.length() - 2);

		int tag = strtol(tagStr.c_str(), nullptr, 2);

		int location1 = -1;
		int location2 = -1;
		if (_RAMTextBoxes[set][1]->getString().length() != 0)
			location1 = strtol(_RAMTextBoxes[set][1]->getString().c_str(), nullptr, 2);
		if (_RAMTextBoxes[set][3]->getString().length() != 0)
			location2 = strtol(_RAMTextBoxes[set][3]->getString().c_str(), nullptr, 2);

		if (location1 == tag || location2 == tag)
		{
			_msg->setString("Cache Hit");
			_msg->setTextColor(glm::vec3(0.0f, 1.0f, 0.0f));
			return;
		}

		_msg->setString("Cache Miss");
		_msg->setTextColor(glm::vec3(1.0f, 0.0f, 0.0f));

		auto fillRAM = [this, &tagStr, &memLoc, &set](bool first)
		{
			int bitwidth = std::ceil(std::log2(_memory.size())) - 2;

			if (tagStr.length() < bitwidth)
			{
				std::string padding = "";
				while (padding.length() + tagStr.length() != bitwidth)
					padding.append("0");

				tagStr = padding + tagStr;
			}

			if (memLoc < _memory.size())
			{
				int tagIndex = !first * 2 + 1;
				_RAMTextBoxes[set][tagIndex]->setString(tagStr);
				_RAMTextBoxes[set][tagIndex + 1]->setString(_memory[memLoc]);

				_RAMTextBoxes[set][tagIndex]->setTextColor(glm::vec3(1.0f, 0.0f, 0.0f));
				_RAMTextBoxes[set][tagIndex + 1]->setTextColor(glm::vec3(1.0f, 0.0f, 0.0f));
				_lastUsed = glm::vec2(set, tagIndex);
			}
			else
			{
				_msg->setString("Invalid Memory Address");
				_msg->setTextColor(glm::vec3(1.0f, 0.2f, 0.5f));
			}
			
		};

		if (location1 == -1 || _oldest[set])
		{
			fillRAM(true);
			_oldest[set] = false;
			return;
		}
		
		if (location2 == -1 || !_oldest[set])
		{
			fillRAM(false);
			_oldest[set] = true;
			return;
		}
		
	});
	_ptrs.simulate = sim;

	glfwSetCursorPosCallback(_window, [](GLFWwindow* win, double x, double y)
	{
		glfwPointers* ptrs = (glfwPointers*)glfwGetWindowUserPointer(win);
		ptrs->file->cursorMoveCallback(win, x, y);
		ptrs->simulate->cursorMoveCallback(win, x, y);
	});
	glfwSetMouseButtonCallback(_window, [](GLFWwindow* win, int button, int action, int mods)
	{
		glfwPointers* ptrs = (glfwPointers*)glfwGetWindowUserPointer(win);
		ptrs->file->mouseCallback(win, button, action, mods);
		ptrs->simulate->mouseCallback(win, button, action, mods);
	});

	_objs.push_back(mainAddr);
	_objs.push_back(mainData);
	_objs.push_back(memTitle);
	_objs.push_back(mainAddrTitle);
	_objs.push_back(mainDataTitle);
	_objs.push_back(RAMtitle);
	_objs.push_back(setTitle);
	_objs.push_back(tagTitle1);
	_objs.push_back(dataTitle1);
	_objs.push_back(tagTitle2);
	_objs.push_back(dataTitle2);
	_objs.push_back(loadFile);
	_objs.push_back(_location);
	_objs.push_back(sim);
	_objs.push_back(_msg);

	_lastUsed = glm::vec2(0.0f, 0.0f);

	glfwSetWindowUserPointer(_window, &_ptrs);
	
}

GLWindow::~GLWindow()
{
	for (auto i : _objs)
	{
		delete i;
	}

	delete holder;
	delete of;
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void GLWindow::start()
{
	for (auto i : _objs)
	{
		i->setup();
	}

	while (!glfwWindowShouldClose(_window))
	{
		double currentTime = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		for (auto i : _objs)
			i->update(currentTime);

		//Draw all objects
		for (auto i : _objs)
			i->draw(_mat);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	glCheckError();
}

void GLWindow::onGLFWError(int error, const char * description)
{
	std::cerr << description << std::endl;
}

