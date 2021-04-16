#pragma once
#include <gl/glew.h>
#include <iostream>
#include <Windows.h>

#ifdef _DEBUG

inline void glCheckError_(const char* file, int line)																
{																					
	GLenum er = glGetError();														
	if(er !=  GL_NO_ERROR)															
		std::cerr << "File: " << file << " Line: " << line << std::endl;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__);

inline void APIENTRY glDebugOutput(GLenum src, GLenum type, unsigned id, GLenum severity,
	GLsizei length, const char* msg, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	std::cerr << msg << std::endl;

	std::cerr << std::endl << std::endl;

	switch (src)
	{
	case GL_DEBUG_SOURCE_API:
		std::cerr << "Source: API";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cerr << "Source: Window System";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cerr << "Source: Shader Compiler";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cerr << "Source: Third Party";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		std::cerr << "Source: Application";
		break;

	case GL_DEBUG_SOURCE_OTHER:
		std::cerr << "Source: Other";
		break;
	}

	std::cerr << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		std::cerr << "Type: Error";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cerr << "Type: Deprecated Behavior";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cerr << "Type: Undefined Behavior";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		std::cerr << "Type: Portability";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cerr << "Type: Performance";
		break;

	case GL_DEBUG_TYPE_MARKER:
		std::cerr << "Type: Marker";
		break;

	case GL_DEBUG_TYPE_PUSH_GROUP:
		std::cerr << "Type: Push Group";
		break;

	case GL_DEBUG_TYPE_POP_GROUP:
		std::cerr << "Type: Pop Group";
		break;

	case GL_DEBUG_TYPE_OTHER:
		std::cerr << "Type: Other";
		break;
	}

	std::cerr << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		std::cerr << "Severity: High";
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cerr << "Severity: Medium";
		break;

	case GL_DEBUG_SEVERITY_LOW:
		std::cerr << "Severity: Low";
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cerr << "Severity: Notification";
		break;
	}

	std::cerr << std::endl << "-------------------------------------------" << std::endl;
}

#else

#define glCheckError()

#endif
