#include <algorithm>
#include "Mp3Frame.h"

Mp3Frame::Mp3Frame() : m_id(""), m_str(""){

}

//constructor from raw id3v2 tag in memory
Mp3Frame::Mp3Frame(ID3V2FRM* frame) : m_str(""), m_id("") {

	if (frame != nullptr) {

		m_id = hxlstr((const uint8_t*)frame->id, 4, hxlstr::ENC::ASCII);
		int payloadSize = GetID3v2PayloadSize(frame);
		if (payloadSize > 0) {
			const uint8_t* payloadAdrs = getTextAddress(frame);
			if (payloadAdrs != nullptr) {
				if (frame->payload.enc == ENC_ASCII) {
					m_str = hxlstr((const uint8_t*)payloadAdrs, payloadSize, hxlstr::ENC::ASCII);
				}
				else if (frame->payload.enc == ENC_UTF16LEWBOM) {
					m_str = hxlstr((const uint8_t*)payloadAdrs, payloadSize, hxlstr::ENC::UTF16LE);
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


//destructor
Mp3Frame::~Mp3Frame() {

}

const Mp3Frame& Mp3Frame::operator=(const Mp3Frame& other) {
	
	m_str = other.m_str;
	m_id = other.m_id;	
	return *this;
}


const hxlstr& Mp3Frame::id() {
	return m_id;
}


const hxlstr& Mp3Frame::text() {
	return m_str;
}
