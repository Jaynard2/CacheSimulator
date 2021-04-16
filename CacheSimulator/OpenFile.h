#pragma once
#include <string>

#include "TextBox.h"

class OpenFileController
{
public:
	OpenFileController(TextBox* target);

	void operator()();

private:
	TextBox* _box;

};
