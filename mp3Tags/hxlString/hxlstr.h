#pragma once

#include <iostream>

enum class ENC {
	ASCII,
	UTF16LE,
	UTF16BE,
	UTF8
};


class hxlstr
{

private:
	char* m_text;
	int m_size;

	int getSize(const char* str);
	int getSize(const char16_t* str);

public:

	enum class ENC {
		ASCII,
		UTF16LE,
		UTF16BE,
	};

	hxlstr();
	hxlstr(const uint8_t* init, int size, ENC enc);
	hxlstr(const char* init);
	hxlstr(const char16_t* init);
	hxlstr(const hxlstr& other);
	hxlstr(char init, int size = 1);
	~hxlstr();
	
	const hxlstr& operator=(const hxlstr& other);
	const hxlstr& operator=(const char* str);
	const hxlstr& operator=(const char16_t* str);
	
	const char* c_str();
	int size();	

	friend std::ostream& operator<<(std::ostream& out, const hxlstr& obj);
	friend bool operator==(const hxlstr& obj1, const hxlstr& obj2);
	friend bool operator!=(const hxlstr& obj1, const hxlstr& obj2);

	
};

