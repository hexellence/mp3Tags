#pragma once

#include <cstdint>
#include "hxlstr.h"
#include "Id3v2Def.h"

using namespace hxl;

class Mp3InfoFrm
{
private:
	hxlstr m_payload;
	hxlstr m_id;

public:
	//Default constructor
	Mp3InfoFrm() : m_payload(""), m_id("") {}

	//Constructor 1
	Mp3InfoFrm(const char* id, const char* text) : m_id(id), m_payload(text) {}
	
	//copy constructor
	Mp3InfoFrm(const Mp3InfoFrm& other) : m_payload(other.m_payload), m_id(other.m_id) {}

	//assignment Operator
	const Mp3InfoFrm& operator=(const Mp3InfoFrm& other)
	{
		m_payload = other.m_payload;
		m_id = other.m_id;
		return *this;
	}

	int exportSize() {
		return sizeof(ID3V2FRMHDR) + m_payload.size();
	}

	const hxlstr& payload() {
		return m_payload;
	}

	~Mp3InfoFrm() {}
};