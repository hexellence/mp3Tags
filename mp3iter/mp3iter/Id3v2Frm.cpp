#include "Id3v2Frm.h"

Id3v2Frm::Id3v2Frm() : m_pfrm(nullptr), m_payloadSize(0), m_enc(ENC::ASCII), m_endian(ENDIAN::LITTLE), m_payload(""), m_id(""), m_pdata(nullptr)  {}

Id3v2Frm::Id3v2Frm(const Id3v2Frm& other) {
	m_pfrm = other.m_pfrm;
	m_payloadSize = other.m_payloadSize;
	m_enc = other.m_enc;
	m_endian = other.m_endian;
	m_payload = other.m_payload;
	m_id = other.m_id;
	if (other.m_pdata != nullptr)
	{
		m_pdata = new uint8_t[m_payloadSize];
		memcpy(m_pdata, other.m_pdata, m_payloadSize);
	}
}

Id3v2Frm::Id3v2Frm(Id3v2Field::FrmHdr* hdr) : m_pfrm(nullptr), m_payloadSize(0), m_enc(ENC::ASCII), m_endian(ENDIAN::LITTLE), m_payload(""), m_id(""), m_pdata(nullptr){
	if (hdr != nullptr) {
		m_pfrm = hdr;		
		char* payload = &hdr->firstChar;
		uint16_t endianness = 0;
		memcpy(&endianness, (uint8_t*)&payload[2], 2);		

		m_payloadSize = getID3v2PayloadSize(hdr);		
		m_id = hxlstr(hdr->id, 4, hxlstr::ENC::ASCII);
		
		if (hdr->id[0] == 'T'){
			if (hdr->firstChar == 0x00) {
				//ascii
				m_payloadSize = m_payloadSize - 1;								
				m_enc = ENC::ASCII;				
				m_payload = hxlstr(&payload[1], m_payloadSize, hxlstr::ENC::ASCII);
			}//if ASCII
			else if (hdr->firstChar == 0x01) {
				//unicode
				m_payloadSize = m_payloadSize - 3;
				m_enc = ENC::UTF16;
				m_payload = hxlstr(&payload[3], m_payloadSize, hxlstr::ENC::UTF16LE);
			}//if unicode
			else {
				std::cout << "unrecocnised encoding" << std::endl;
			}
		}//text field
		else {
			//do a blind copy
			m_pdata = new uint8_t[m_payloadSize];
			memcpy(m_pdata, &hdr->firstChar, m_payloadSize);
		}
	}
};

Id3v2Frm::~Id3v2Frm() {
	delete[] m_pdata;	
};

const Id3v2Frm& Id3v2Frm::operator=(const Id3v2Frm& other) {

	m_pfrm = other.m_pfrm;
	m_payloadSize = other.m_payloadSize;
	m_enc = other.m_enc;
	m_endian = other.m_endian;
	m_payload = other.m_payload;
	m_id = other.m_id;
	if (other.m_pdata != nullptr)
	{
		delete[] m_pdata;
		m_pdata = new uint8_t[m_payloadSize];
		memcpy(m_pdata, other.m_pdata, m_payloadSize);
	}
	return *this;
}

hxlstr& Id3v2Frm::getFrameText() {
	return m_payload;
}


uint8_t* Id3v2Frm::getFrameData() {
	return m_pdata;
}

hxlstr& Id3v2Frm::getFrameId() {
	return m_id;
}

int Id3v2Frm::readID3v2FieldSize(const char* frm) {

	return  getID3v2PayloadSize((const Id3v2Field::FrmHdr*)frm) + 10;
}

int Id3v2Frm::getID3v2PayloadSize(const Id3v2Field::FrmHdr* frame) {
	int payloadSize = 0;

	if (frame != nullptr) {
		//frame payload sizes may sometimes not according to the standard
		//check first if it is according to the standard
		payloadSize = calcID3v2SizeField(frame->payloadSize);

		Id3v2Field::FrmHdr* nextFrmAddress = (Id3v2Field::FrmHdr*)((int)frame + payloadSize + ID3V2_HDR_SIZE);

		if (!isID3v2FieldIdValid(nextFrmAddress)) {
			//if not valid check it without the standard way
			payloadSize = calcID3v2SizeField(frame->payloadSize, true);
			nextFrmAddress = (Id3v2Field::FrmHdr*)((int)frame + payloadSize + ID3V2_HDR_SIZE);
		}

		if (!isID3v2FieldIdValid(nextFrmAddress)) 
		{
			nextFrmAddress = nullptr;
		}
	}
	return payloadSize;
}