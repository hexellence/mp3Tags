#pragma once
//
//#include "hxlstr.h"
//#include "Id3v2Field.h"
//
//static const uint8_t ENC_ASCII = 0;
//static const uint8_t ENC_UTF16 = 1;
//
//static const uint16_t ENDIAN_LITTLE = 0xFFFE;
//static const uint16_t ENDIAN_BIG = 0xFEFF;
//
//using namespace hxl;
//
//
//class Id3v2Frm : public Id3v2Field {
//
//	enum class ENC {
//		ASCII,
//		UTF16	
//	};
//
//	enum class ENDIAN {
//		LITTLE = 0xFFFE,
//		BIG = 0xFEFF,
//	};
//
//private:
//	Id3v2Field::FrmHdr* m_pfrm;
//	int m_payloadSize;
//	ENC m_enc;
//	ENDIAN m_endian;
//	hxlstr m_payload;
//	hxlstr m_id;
//	uint8_t* m_pdata;
//	int getID3v2PayloadSize(const Id3v2Field::FrmHdr* frame);
//	int readID3v2FieldSize(const char* frm);
//	
//
//
//public:
//	Id3v2Frm(Id3v2Field::FrmHdr* hdr);
//	Id3v2Frm();
//	Id3v2Frm(const Id3v2Frm& other);
//	~Id3v2Frm();
//	const Id3v2Frm& operator=(const Id3v2Frm& other);
//	hxlstr& getFrameText();
//	hxlstr& getFrameId();
//	uint8_t* getFrameData();
//
//	
//	
//};
//
