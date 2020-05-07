#include "Mp3Tag.h"

Mp3Tag::Mp3Tag(std::filesystem::path filePath) : m_size(0), m_firstFrame(nullptr), m_id3Header(nullptr){

	ID3V2HDR id3v2Hdr;
	std::ifstream mp3File;
	mp3File.open(filePath, std::ios::binary);

	if (mp3File.is_open()) {
		mp3File.seekg(std::ios::beg);
		mp3File.read((char*)&id3v2Hdr, sizeof(id3v2Hdr));	//only read header first for effectiveness		
		hxlstr id((const uint8_t*)id3v2Hdr.id, 3, hxlstr::ENC::ASCII);
		m_fileName = filePath.filename().generic_u16string().c_str();
		m_filePath = filePath.generic_u16string().c_str();

		if (id == "ID3") {
			m_size = calcID3FieldSize((const uint8_t*)id3v2Hdr.size);
			if (m_size > 0) {
				uint8_t* m_buffer = new uint8_t[m_size];
				mp3File.seekg(std::ios::beg);
				mp3File.read((char*)m_buffer, m_size);
				mp3File.close();
				m_id3Header = (ID3V2HDR*)m_buffer;
								
				m_firstFrame = &m_id3Header->firstFrame;
				iterateFrames();
				delete[] m_buffer;
			}
			else {
				std::cout << "ID3 size too big for file: " << m_fileName << std::endl;
			}

			if (id3v2Hdr.flags != 0x00) {
				m_size = 0;
				std::cout << "Unexpected Tag flags are set: " << m_fileName << std::endl;
			}
		}
		else {
			std::cout << "Not ID3V2 compatible: " << m_fileName << std::endl;
		}
	}
	else {
		std::cout << "Mp3 cannot be opened" << std::endl;
	}
}


Mp3Tag::~Mp3Tag() {

}


ID3V2FRM* Mp3Tag::getNextFrame(ID3V2FRM* currentFrame) {

	ID3V2FRM* nextFrame = nullptr;

	int payloadSize = calcID3FieldSize(currentFrame->payloadSize);

	nextFrame = (ID3V2FRM*)((int)currentFrame + 10 + payloadSize);

	return nextFrame;
}


void Mp3Tag::iterateFrames() {

	ID3V2FRM* p_id3v2NextFrm = nullptr;
	ID3V2FRM* p_id3v2CurrFrm = m_firstFrame;

	if (m_size > 0) {

		while (isIdValid(*p_id3v2CurrFrm)) {
			Mp3Frame curFrame = Mp3Frame(p_id3v2CurrFrm);

			if (curFrame.id() == "TYER") {
				m_year = curFrame.text();
			}
			else if (curFrame.id() == "TALB")
			{
				m_album = curFrame.text();
			}
			else if (curFrame.id() == "TIT2")
			{
				m_title = curFrame.text();
			}
			else if (curFrame.id() == "TRCK")
			{
				m_trackNo = curFrame.text();
			}
			else if (curFrame.id() == "TCON")
			{
				m_genre = curFrame.text();
			}
			else if (curFrame.id() == "TPE2")
			{
				m_artist1 = curFrame.text();
			}
			else if (curFrame.id() == "TPE1")
			{
				m_artist2 = curFrame.text();
			}

			p_id3v2CurrFrm = getNextFrame(p_id3v2CurrFrm);
			
		}	//while
	}//if size is non zero
}