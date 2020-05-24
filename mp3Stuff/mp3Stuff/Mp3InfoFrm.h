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
	
	//Constructor 2
	Mp3InfoFrm(ID3V2FRMHDR* frame) 
	{
		if (frame != nullptr) {

			m_id = hxlstr((const uint8_t*)frame->id, 4, hxlstr::ENC::ASCII);
			int payloadSize = GetID3v2PayloadSize(frame);
			if (payloadSize > 0) {
				const uint8_t* payloadAdrs = getID3v2PayloadTextAddress(frame);
				if (payloadAdrs != nullptr) {
					if (payloadAdrs[0] == ENC_ASCII) {
						m_payload = hxlstr((const uint8_t*)payloadAdrs, payloadSize, hxlstr::ENC::ASCII);
						m_payload = clean(m_payload);
					}
					else if (payloadAdrs[0] == ENC_UTF16LEWBOM) {
						m_payload = hxlstr((const uint8_t*)payloadAdrs, payloadSize, hxlstr::ENC::UTF16LE);
						m_payload = clean(m_payload);
					}
					else {
						std::cout << "Mp3Frame::Mp3Frame: Illegal encoding type" << std::endl;
					}					
				}
			}
		}
		else {
			throw std::string("no frame address!");
		}
	}
	

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

//utils
hxlstr& clean(hxlstr& in) {
	in.trim(u" \t\x0d\x0a");
	in.remove(u'\0');	//removes inbetween 
	in.remove(u'\t');	//removes inbetween 
	in.remove(u'\r');	//removes inbetween 
}
