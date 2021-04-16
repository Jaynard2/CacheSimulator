#include "RenderString.h"
#include <algorithm>

RenderString::RenderString(std::shared_ptr<GLRenderingProgram> prog) : RenderObject(prog), std::string(), _width(0)
{
	RenderString::generateRenderString();
}

RenderString::RenderString(std::shared_ptr<GLRenderingProgram> prog, std::string str) : 
	RenderObject(prog), std::string(str), _width(0)
{
	RenderString::generateRenderString();
}

RenderString::RenderString(std::shared_ptr<GLRenderingProgram> prog, std::string str, glm::vec2 pos) :
	RenderObject(prog), std::string(str), _pos(pos), _width(0)
{
	RenderString::generateRenderString();
}

void RenderString::draw(const glm::mat4 & matrix)
{
	_width = 0.0f;
	glm::vec2 offset = glm::vec2(0, 0);
	for (auto i : _str)
	{
		i.setColor(_color);

		if (i.getChar() != '\n')
		{
			i.setLoc(_pos + offset);
			i.draw(matrix);

			offset.x += Character::getAdvance(i.getChar()) >> 6;
		}
		else
		{
			_width = std::max(_width, offset.x);

			offset.x = 0;
			offset.y -= Character::getAdvanceY();
		}
	}

	_width = std::max(_width, offset.x);
}

void RenderString::setup()
{
	generateRenderString();
}

void RenderString::update(double time)
{
	if (!_reformat.empty())
		generateRenderString();
}

void RenderString::clear() noexcept
{
	_str.clear();
	std::string::clear();
}

RenderString & RenderString::operator=(const std::string & str)
{
	std::string::operator=(str);
	generateRenderString();

	return *this;
}

RenderString & RenderString::operator=(const char * s)
{
	std::string::operator=(s);
	generateRenderString();

	return *this;
}

RenderString & RenderString::operator=(char c)
{
	std::string::operator=(c);
	generateRenderString();

	return *this;
}

RenderString & RenderString::operator=(std::initializer_list<char> il)
{
	std::string::operator=(il);
	generateRenderString();

	return *this;
}

RenderString & RenderString::operator=(std::string && str) noexcept
{
	std::string::operator=(str);
	generateRenderString();

	return *this;
}

char & RenderString::operator[](size_t pos)
{
	_reformat.emplace(pos);
	return std::string::operator[](pos);
}

const char & RenderString::operator[](size_t pos) const
{
	return std::string::operator[](pos);
}

char & RenderString::at(size_t pos)
{
	_reformat.emplace(pos);
	return std::string::at(pos);
}

const char & RenderString::at(size_t pos) const
{
	return std::string::at(pos);
}

char & RenderString::back()
{
	_reformat.emplace(length() - 1);
	return std::string::back();
}

const char & RenderString::back() const
{
	return std::string::back();
}

char & RenderString::front()
{
	_reformat.emplace(0);
	return std::string::front();
}

const char & RenderString::front() const
{
	return std::string::front();
}

RenderString & RenderString::operator+=(const std::string & str)
{
	for (unsigned i = 0; i < str.length(); i++)
	{
		_str.push_back(Character(str[i], p_prog));
		_str.back().setup();
	}

	std::string::operator+=(str);
	return *this;
}

RenderString & RenderString::operator+=(const char * s)
{
	for (const char* i = s; *i != '\0'; i++)
	{
		_str.push_back(Character(*i, p_prog));
		_str.back().setup();
	}

	std::string::operator+=(s);
	return *this;
}

RenderString & RenderString::operator+=(char c)
{
	_str.push_back(Character(c, p_prog));
	_str.back().setup();

	std::string::operator+=(c);
	return *this;
}

RenderString & RenderString::operator+=(std::initializer_list<char> il)
{
	for (auto i = il.begin(); i != il.end(); i++)
	{
		_str.push_back(Character(*i, p_prog));
		_str.back().setup();
	}

	std::string::operator+=(il);
	return *this;
}

RenderString & RenderString::append(const std::string & str)
{
	for (unsigned i = 0; i < str.length(); i++)
	{
		_str.push_back(Character(str[i], p_prog));
		_str.back().setup();
	}

	std::string::append(str);
	return *this;
}

RenderString & RenderString::append(const std::string & str, size_t subpos, size_t sublen)
{
	for (auto i = subpos; i != sublen && i != str.npos; i++)
	{
		_str.push_back(Character(str[i], p_prog));
		_str.back().setup();
	}

	std::string::append(str, subpos, sublen);
	return *this;
}

RenderString & RenderString::append(const char * s)
{
	for (const char* i = s; *i != '\0'; i++)
	{
		_str.push_back(Character(*i, p_prog));
		_str.back().setup();
	}

	std::string::append(s);
	return *this;
}

RenderString & RenderString::append(const char * s, size_t n)
{
	for (const char* i = s; i != s + n && *i != '\n'; i++)
	{
		_str.push_back(Character(*i, p_prog));
		_str.back().setup();
	}

	std::string::operator+=(s);
	return *this;
}

RenderString & RenderString::append(size_t n, char c)
{
	for (unsigned i = 0; i < n; i++)
	{
		_str.push_back(Character(c, p_prog));
		_str.back().setup();
	}

	std::string::append(n, c);
	return *this;
}

RenderString & RenderString::append(std::initializer_list<char> il)
{
	for (auto i = il.begin(); i != il.end(); i++)
	{
		_str.push_back(Character(*i, p_prog));
		_str.back().setup();
	}

	std::string::append(il);
	return *this;
}

void RenderString::push_back(char c)
{
	_str.push_back(Character(c, p_prog));
	_str.back().setup();
	std::string::push_back(c);
}

RenderString & RenderString::insert(size_t pos, const std::string & str)
{
	for (auto i = 0; i < str.length(); i++)
	{
		auto iter = _str.insert(_str.begin() + pos + i, Character(str[i], p_prog));
		iter->setup();
	}

	std::string::insert(pos, str);
	return *this;	
}

RenderString & RenderString::insert(size_t pos, const std::string & str, size_t subpos, size_t sublen)
{
	for (auto i = subpos; i < sublen && i != str.npos; i++)
	{
		auto iter = _str.insert(_str.begin() + pos + i, Character(str[i], p_prog));
		iter->setup();
	}

	std::string::insert(pos, str, subpos, sublen);
	return *this;
}

RenderString & RenderString::insert(size_t pos, const char * s)
{
	for (auto i = 0; s[i] != '\n'; i++)
	{
		auto iter = _str.insert(_str.begin() + pos + i, Character(s[i], p_prog));
		iter->setup();
	}

	std::string::insert(pos, s);
	return *this;
}

RenderString & RenderString::insert(size_t pos, const char * s, size_t n)
{
	for (auto i = 0; s[i] != '\n' && i != n; i++)
	{
		auto iter = _str.insert(_str.begin() + pos + i, Character(s[i], p_prog));
		iter->setup();
	}

	std::string::insert(pos, s);
	return *this;
}

RenderString & RenderString::insert(size_t pos, size_t n, char c)
{
	for (auto i = 0; i < n; i++)
	{
		auto iter = _str.insert(_str.begin() + pos + i, Character(c, p_prog));
		iter->setup();
	}

	std::string::insert(pos, n, c);
	return *this;
}

RenderString & RenderString::erase(size_t pos, size_t len)
{
	_str.erase(_str.begin() + pos, _str.begin() + pos + len);
	std::string::erase(pos, len);

	return *this;
}

RenderString::iterator RenderString::erase(const_iterator p)
{
	unsigned index = (unsigned)(p - begin());
	_str.erase(_str.begin() + index);

	return std::string::erase(p);
}

RenderString::iterator RenderString::erase(const_iterator first, const_iterator last)
{
	unsigned start = (unsigned)(first - begin());
	unsigned end = (unsigned)(last - begin());
	_str.erase(_str.begin() + start, _str.begin() + end);

	return std::string::erase(first, last);
	
}

void RenderString::pop_back()
{
	_str.pop_back();
	std::string::pop_back();
}

void RenderString::generateRenderString()
{
	if (_reformat.empty())
	{
		_str.clear();

		for (const auto& i : *this)
		{
			_str.push_back(Character(i, p_prog, Character::getFontSize()));
			_str.back().setup();
		}
	}
	else
	{
		while (!_reformat.empty())
		{
			_str[_reformat.front()] = Character(std::string::at(_reformat.front()), p_prog);
			_str[_reformat.front()].setup();

			_reformat.pop();
		}
	}
}
