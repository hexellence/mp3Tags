
#include <algorithm>
#include "Mp3Frame.h"


Mp3Frame::Mp3Frame() : m_str("") { }

//constructor from raw id3v2 tag in memory
Mp3Frame::Mp3Frame(ID3V2FRM* frame) : m_str(""), m_id("") {

	if (frame != nullptr) {

		m_id = hxlstr((const uint8_t*)frame->id, 4, hxlstr::ENC::ASCII);
		int payloadSize = GetID3v2PayloadSize(frame);
		if (payloadSize > 0) {
			const uint8_t* payloadAdrs = getTextAddress(frame);
			if (payloadAdrs != nullptr) {
				
				m_str = hxlstr((const uint8_t*)payloadAdrs, payloadSize, hxlstr::ENC::ASCII);
				bool idFound = false;
				for (auto item : ID3_FrameDefs) {
					if (m_id == item.sLongTextID) {
						idFound = true;
						break;
					}
				}
			}
		}
	}
	else {
		throw std::string("no frame address!");
	}
}

//constructor from normal char array
Mp3Frame::Mp3Frame(const char* init, const char* id) : m_str(""), m_id("") {
	if ((init != nullptr) && (id != nullptr)) {
		m_str = hxlstr(init);
		m_id = hxlstr(id);
	}
	else {
		throw std::string("no array address!");
	}
}

//copy constructor
Mp3Frame::Mp3Frame(const Mp3Frame& other) : m_str(other.m_str), m_id(other.m_id) {

}

//destructor
Mp3Frame::~Mp3Frame() {

}

const hxlstr& Mp3Frame::id() {
	return m_id;
}

const hxlstr& Mp3Frame::text() {
	return m_str;
}

//assignment operator
const Mp3Frame& Mp3Frame::operator=(const Mp3Frame& other) {
	m_str = other.m_str;
	m_id = other.m_id;
	return *this;
}

//assignment operator from char16_t
const Mp3Frame& Mp3Frame::operator=(const char16_t* other) {
	m_str = hxlstr(other);
	m_id = "NONE";
	return *this;
}
