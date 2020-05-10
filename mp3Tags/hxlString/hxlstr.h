#pragma once

#include <iostream>
#include "trim.h"

namespace hxl {

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
		const char16_t* m_ptr;

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
		hxlstr(int number);
		~hxlstr();

		const hxlstr& operator=(const hxlstr& other);
		const hxlstr& operator=(const char* str);
		const hxlstr& operator=(const char16_t* str);

		const char* c_str() const;
		int size() const;
		int rtrim(const char16_t* chars = u" \t");
		int ltrim(const char16_t* chars = u" \t");
		int trim(const char16_t* chars = u" \t");
		

		friend std::ostream& operator<<(std::ostream& out, const hxlstr& obj);
		friend bool operator==(const hxlstr& obj1, const hxlstr& obj2);
		friend bool operator!=(const hxlstr& obj1, const hxlstr& obj2);


	};

}//namespace hxl