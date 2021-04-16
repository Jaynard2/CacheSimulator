#include "FreeTypeException.h"

FreeTypeException::FreeTypeException(std::string what) : std::runtime_error(what)
{
}
