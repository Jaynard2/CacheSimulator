#include <iostream>
#include <Windows.h>
#include <string>

#include "GLWindow.h"
#include "OpenGLFailure.h"

//Request use of high performace graphics cards
extern "C"
{
	//Select NVIDIA card if avaible
	_declspec (dllexport) DWORD NvOptimusEnablement = 1;
	//Select AMD card if available
	_declspec (dllexport) DWORD AmdPowerXpressRequestHighPerformance = 1;
}

int main()
{
	try
	{
		GLWindow window("Cache Simulator", 1280, 720);
		window.start();
	}
	catch (const OpenGLFailure& er)
	{
		std::cerr << er.what();
		return -1;
	}

	return 0;
}
