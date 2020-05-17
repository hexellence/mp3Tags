#include <algorithm>
#include "Mp3Frame.h"
#include "fileSys.h"

using namespace hxl;

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
					m_str.trim(u" \t\x0d\x0a");
					m_str.remove(u'\0');	//removes inbetween 
					m_str.remove(u'\t');	//removes inbetween 
					m_str.remove(u'\r');	//removes inbetween 
				}
				else if (frame->payload.enc == ENC_UTF16LEWBOM) {
					m_str = hxlstr((const uint8_t*)payloadAdrs, payloadSize, hxlstr::ENC::UTF16LE);
					m_str.trim(u" \t\x0d\x0a");
					m_str.remove(u'\0');	//removes inbetween 
					m_str.remove(u'\t');	//removes inbetween 
					m_str.remove(u'\r');	//removes inbetween 
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


Mp3Frame::Mp3Frame(hxlstr& val, hxlstr& id) : m_str(val), m_id(id) { }

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

//Helpers

int createID3v2Frame(Mp3Frame& frameIn, char* frameOut) {
	int frmsize = 0;

	if (frameOut != nullptr) {
		ID3V2FRM* p_id3V2Frm = (ID3V2FRM*)frameOut;
		hxlstrcopy(p_id3V2Frm->id, frameIn.id());
		frmsize = frmsize + sizeof(p_id3V2Frm->id);		

		p_id3V2Frm[0].flags[0] = 0x00;
		p_id3V2Frm[0].flags[1] = 0x00;
		frmsize = frmsize + sizeof(p_id3V2Frm->flags);

		//BOM prepare
		p_id3V2Frm->payload.enc = 0x01;
		frmsize = frmsize + sizeof(p_id3V2Frm->payload.enc);
		//BOM 
		char* p_BOM = &p_id3V2Frm->payload.txt;
		memcpy(p_BOM, BOM, 2);
		frmsize = frmsize + 2;

		// to ease coding have a pointer to the actual payload
		char* p_Payload = &p_BOM[2];

		// copy album name
		memcpy(p_Payload, (char*)frameIn.text().c_str(), frameIn.text().size());
		frmsize = frmsize + frameIn.text().size();

		// payload size field
		frmsize = frmsize + sizeof(p_id3V2Frm->payloadSize);

		//set payload size
		uint32_t payloadSize = frameIn.text().size() + 3;
		memcpy(p_id3V2Frm->payloadSize, (uint8_t*)&payloadSize, 4);
		
		//convert endianness
		convertEndianness(p_id3V2Frm->payloadSize, 4);				
	}
	return frmsize;
}