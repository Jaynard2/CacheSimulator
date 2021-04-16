#pragma once
#include <stdexcept>
#include <string>

class FreeTypeException : public std::runtime_error
{
public:
	FreeTypeException(std::string what = "");
};

