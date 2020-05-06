#include "Mp3Tag.h"


Mp3Tag::Mp3Tag(const Mp3Tag& other) {

}


Mp3Tag::Mp3Tag(std::filesystem::path filePath) : m_size(0), m_firstFrame(nullptr) {

	ID3V2HDR id3v2Hdr;
	m_file.open(filePath, std::ios::binary);

	if (m_file.is_open()) {
		m_file.seekg(std::ios::beg);
		m_file.read((char*)&id3v2Hdr, sizeof(id3v2Hdr));	//only read header first for effectiveness		
		hxlstr id((const uint8_t*)id3v2Hdr.id, 3, hxlstr::ENC::ASCII);

		if (id == "ID3") {
			m_size = calcID3FieldSize((const uint8_t*)id3v2Hdr.size);
			if ((m_size > 0) && (m_size < ID3_TAG_MAX_SIZE)) {
				uint8_t* m_buffer = new uint8_t[m_size];
				m_file.seekg(std::ios::beg);
				m_file.read((char*)m_buffer, m_size);
				m_file.close();
				m_id3Header = (ID3V2HDR*)m_buffer;

				m_fileName = filePath.filename().generic_u16string().c_str();
				m_filePath = filePath.generic_u16string().c_str();
				m_firstFrame = &m_id3Header->firstFrame;
				iterateFrames();
				delete[] m_buffer;
			}
			else {
				std::cout << "ID3 size too big" << std::endl;
			}

			if (id3v2Hdr.flags != 0x00) {
				m_size = 0;
				std::cout << "Unexpected Tag flags are set" << std::endl;
			}
		}
		else {
			std::cout << "Not ID3V2 compatible" << std::endl;
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

		Mp3Frame curFrame(p_id3v2CurrFrm);
		while (curFrame.id() != hxlstr('\0', 4)) {

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

			p_id3v2NextFrm = getNextFrame(p_id3v2CurrFrm);
			p_id3v2CurrFrm = p_id3v2NextFrm;
			curFrame = Mp3Frame(p_id3v2CurrFrm);
		}	//while
	}//if size is non zero
}