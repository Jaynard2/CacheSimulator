#pragma once
#include <String>
#include <memory>
#include <vector>
#include <queue>

#include "RenderObject.h"
#include "Character.h"
#include "GLRenderingProgram.h"

class RenderString : public RenderObject, public std::string
{
public:
	RenderString(std::shared_ptr<GLRenderingProgram> prog);
	RenderString(std::shared_ptr<GLRenderingProgram> prog, std::string str);
	RenderString(std::shared_ptr<GLRenderingProgram> prog, std::string str, glm::vec2 pos);

	void setPosition(glm::vec2 pos) { _pos = pos; }
	void setColor(glm::vec3 color) { _color = color; }

	unsigned getFontSize() const { return Character::getFontSize(); }
	float getWidth() const { return _width; }

	virtual void draw(const glm::mat4& matrix) override;
	virtual void setup() override;
	virtual void update(double time) override;

	void clear() noexcept;

	RenderString& operator= (const std::string& str);
	RenderString& operator= (const char* s);
	RenderString& operator= (char c);
	RenderString& operator= (std::initializer_list<char> il);
	RenderString& operator= (std::string&& str) noexcept;

	char& operator[] (size_t pos);
	const char& operator[] (size_t pos) const;

	char& at(size_t pos);
	const char& at(size_t pos) const;

	char& back();
	const char& back() const;

	char& front();
	const char& front() const;

	RenderString& operator+= (const std::string& str);
	RenderString& operator+= (const char* s);
	RenderString& operator+= (char c);
	RenderString& operator+= (std::initializer_list<char> il);

	RenderString& append(const std::string& str);
	RenderString& append(const std::string& str, size_t subpos, size_t sublen = npos);
	RenderString& append(const char* s);
	RenderString& append(const char* s, size_t n);
	RenderString& append(size_t n, char c);
	RenderString& append(std::initializer_list<char> il);

	void push_back(char c);

	RenderString& insert(size_t pos, const std::string& str);
	RenderString& insert(size_t pos, const std::string& str, size_t subpos, size_t sublen = npos);
	RenderString& insert(size_t pos, const char* s);
	RenderString& insert(size_t pos, const char* s, size_t n);
	RenderString& insert(size_t pos, size_t n, char c);

	RenderString& erase(size_t pos = 0, size_t len = npos);
	iterator erase(const_iterator p);
	iterator erase(const_iterator first, const_iterator last);

	void pop_back();

private:
	float _width;
	std::vector<Character> _str;
	glm::vec2 _pos;
	std::queue<unsigned> _reformat;
	glm::vec3 _color;

	void generateRenderString();

};

