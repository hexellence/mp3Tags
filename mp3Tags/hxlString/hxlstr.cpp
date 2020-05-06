#include "hxlstr.h"

void convert_char_to_char16(const uint8_t* in, int size, uint8_t* out) {
	for (int i = 0; i < size; ++i) {
		out[i * 2] = in[i];
		out[(i * 2) + 1] = '\0';
	}
}

void convert_char16_to_char(const uint8_t* in, int size, uint8_t* out) {
	for (int i = 0; i < size; ++i) {
		out[i] = in[i * 2];
	}

}

int hxlstr::getSize(const char* str) {
	int len = 0;

	if (str != nullptr) {
		while (str[len] != '\0') {
			len++;
		}
	}
	return len;
}

int hxlstr::getSize(const char16_t* str) {
	int len = 0;
	if (str != nullptr) {
		while (str[len] != u'\0') {
			len++;
		}
	}
	return len * 2;
}

hxlstr::hxlstr() : m_text(nullptr), m_size(0) {}

hxlstr::hxlstr(const uint8_t* init, int size, ENC enc) : m_text(nullptr), m_size(0) {

	if ((init != nullptr) && (size > 0)) {
		switch (enc) {
		case ENC::ASCII:
			m_size = 2 * size;
			m_text = new char[m_size + 2];
			convert_char_to_char16(init, size, (uint8_t*)m_text);
			m_text[m_size] = '\0';
			m_text[m_size + 1] = '\0';
			break;
		case ENC::UTF16LE:
			m_size = size;
			m_text = new char[m_size + 2];
			memcpy(m_text, init, m_size);
			m_text[m_size] = '\0';
			m_text[m_size + 1] = '\0';
			break;
		default:
			break;
		}
	}
	else {
		std::cout << "Illegal parameters" << std::endl;
	}
}

hxlstr::hxlstr(const char* init) : m_text(nullptr), m_size(0) {
	//we expect to find a terminator here
	if (init != nullptr) {
		int size = getSize(init);
		m_size = 2 * size;
		m_text = new char[m_size + 2];
		convert_char_to_char16((const uint8_t*)init, size, (uint8_t*)m_text);
		m_text[m_size] = '\0';
		m_text[m_size + 1] = '\0';
	}
	else {
		std::cout << "illegal parameters" << std::endl;
	}
}

hxlstr::hxlstr(char init, int size) {
	char* tempInit = new char[size];
	//fill the array
	for (int i = 0; i < size; ++i) {
		tempInit[i] = init;
	}

	m_size = 2 * size;
	m_text = new char[m_size + 2];
	convert_char_to_char16((const uint8_t*)tempInit, size, (uint8_t*)m_text);
	delete[] tempInit;
	m_text[m_size] = '\0';
	m_text[m_size + 1] = '\0';
}

hxlstr::hxlstr(const char16_t* init) : m_text(nullptr), m_size(0) {
	//we expect a terminator here
	if (init != nullptr) {
		m_size = getSize(init);
		m_text = new char[m_size + 2];	//len didn't include terminators
		memcpy(m_text, (const char*)init, m_size + 2);		//since the original value has two buye termination copy size will include that too
	}
}

hxlstr::hxlstr(const hxlstr& other) {
	m_size = other.m_size;
	m_text = new char[m_size + 2];
	memcpy(m_text, other.m_text, m_size + 2);
}

const hxlstr& hxlstr::operator=(const hxlstr& other) {
	delete[] m_text;
	m_size = other.m_size;
	m_text = new char[m_size + 2];
	memcpy(m_text, other.m_text, m_size + 2);
	return *this;
}

const hxlstr& hxlstr::operator=(const char* str) {
	delete[] m_text;
	int sourceSize = getSize(str);
	m_size = sourceSize * 2;
	m_text = new char[m_size + 2];
	convert_char_to_char16((const uint8_t*)str, sourceSize, (uint8_t*)m_text);
	m_text[m_size] = '\0';
	m_text[m_size + 1] = '\0';
	return *this;
}

const hxlstr& hxlstr::operator=(const char16_t* str) {
	delete[] m_text;
	int sourceSize = getSize(str);
	m_size = sourceSize;
	m_text = new char[m_size + 2];
	memcpy(m_text, (const char*)str, m_size + 2);
	return *this;
}

hxlstr::~hxlstr() {
	delete[] m_text;
}

const char* hxlstr::c_str() {
	return m_text;
}

int hxlstr::size() {
	return m_size;
}

//Fiends
std::ostream& operator<<(std::ostream& out, const hxlstr& obj) {
	if ((obj.m_text != nullptr) && (obj.m_size > 0)) {
		char* tempText = new char[obj.m_size];
		convert_char16_to_char((const uint8_t*)obj.m_text, obj.m_size, (uint8_t*)tempText);
		out << tempText << std::flush;
		delete[] tempText;
	}
	return out;
}

bool operator==(const hxlstr& obj1, const hxlstr& obj2) {
	bool retVal = false;
	if (obj1.m_size == obj2.m_size) {
		retVal = true;
		for (int i = 0; i < obj1.m_size; ++i) {
			if (obj1.m_text[i] != obj2.m_text[i]) {
				retVal = false;
				break;
			}
		}
	}
	else {
		retVal = false;
	}
	return retVal;
}


bool operator!=(const hxlstr& obj1, const hxlstr& obj2) {
	
	bool retVal = !(obj1 == obj2);
	return retVal;
}


