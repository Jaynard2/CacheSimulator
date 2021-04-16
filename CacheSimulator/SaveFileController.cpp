#include "SaveFileController.h"
#include <nfd.h>
#include <fstream>

SaveFileController::SaveFileController(TextBox * src, std::string fileType) : _src(src), _type(fileType)
{
}

void SaveFileController::operator()()
{
	nfdchar_t* savePath = nullptr;
	nfdresult_t result = NFD_SaveDialog(_type.c_str(), nullptr, &savePath);

	if (result == NFD_OKAY)
	{
		std::string fileName = std::string(savePath);
		if (fileName.find(".") == fileName.npos)
			fileName += "." + _type;

		std::fstream write(fileName, std::fstream::out);

		write << _src->getString();

		write.close();
	}
}
