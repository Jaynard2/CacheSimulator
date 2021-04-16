#include "OpenFile.h"
#include <nfd.h>
#include <fstream>

OpenFileController::OpenFileController(TextBox * target) : _box(target)
{
}

void OpenFileController::operator()()
{
	nfdchar_t* outPath = nullptr;
	nfdresult_t result = NFD_OpenDialog("csv", nullptr, &outPath);

	if (result == NFD_OKAY)
	{
		std::fstream file(outPath);
		
		std::string in = "";
		while (!file.eof())
		{
			std::string temp = "";
			std::getline(file, temp);
			in += temp + '\n';
		}

		in.pop_back();
		_box->setString(in);
		
		file.close();
	}
}
