#pragma once
#include <string>

#include "TextBox.h"

class SaveFileController
{
public:
	SaveFileController(TextBox* src, std::string fileType);

	void operator()();
	
private:
	TextBox* _src;
	std::string _type;
};

