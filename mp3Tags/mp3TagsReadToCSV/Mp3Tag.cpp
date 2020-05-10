#include <fstream>
#include "Mp3Tag.h"
#include "ID3V1Genre.h"



Mp3Tag::Mp3Tag(std::filesystem::path filePath) : 
	m_size(0),
	m_firstFrame(nullptr),
	m_status (STATUS::MP3TAG_SUCCESS) {

	ID3V2HDR id3v2Hdr;
	ID3V1HDR id3v1Hdr;

	std::ifstream mp3File;	

	if (getID3Header(filePath, &id3v2Hdr, &id3v1Hdr)) {

		hxlstr id((const uint8_t*)id3v2Hdr.id, 3, hxlstr::ENC::ASCII);
		hxlstr tag((const uint8_t*)id3v1Hdr.tag, 3, hxlstr::ENC::ASCII);

		m_fileName = filePath.filename().generic_u16string().c_str();
		m_filePath = filePath.generic_u16string().c_str();
		hxlstr fileExt = filePath.extension().generic_u16string().c_str();

		if (isMp3(filePath) == true) {

			std::cout << m_filePath << std::endl;

			if (id == "ID3") {
				m_size = calcID3FieldSize(id3v2Hdr.size);
				if (m_size > 0) {
					uint8_t* tagBuffer = new uint8_t[m_size + 10];	//size in file is excluding the header
					mp3File.open(filePath, std::ios::binary);
					mp3File.seekg(0, std::ios::beg);
					mp3File.read((char*)tagBuffer, (long long)m_size + 10);
					mp3File.close();
					
					ID3V2HDR* id3v2Header = (ID3V2HDR*)tagBuffer;

					m_firstFrame = &id3v2Header->firstFrame;
					iterateFrames();
					delete[] tagBuffer;
				}
				else {
					std::cout << "ID3 size wrong for file: " << m_fileName << std::endl;
				}

				if (id3v2Hdr.flags != 0x00) {
					m_size = 0;
					m_status = STATUS::MP3TAG_ILLFORMED_ID3V2;
				}
			}

			if (tag == "TAG") {
				if (id3v1Hdr.zero == 0x00) {

					if (m_title.size() == 0) {
						m_title = hxlstr((uint8_t*)id3v1Hdr.title, sizeof(id3v1Hdr.title), hxlstr::ENC::ASCII);
						m_title.trim();
					}
					if (m_album.size() == 0) {
						m_album = hxlstr((uint8_t*)id3v1Hdr.album, sizeof(id3v1Hdr.album), hxlstr::ENC::ASCII);
						m_album.trim();
					}
					if (m_artist1.size() == 0) {
						m_artist1 = hxlstr((uint8_t*)id3v1Hdr.artist, sizeof(id3v1Hdr.artist), hxlstr::ENC::ASCII);
						m_artist1.trim();
					}
					if (m_artist2.size() == 0) {
						m_artist2 = m_artist1;
					}
					if (m_year.size() == 0) {
						m_year = hxlstr((const uint8_t*)id3v1Hdr.year, 4, hxlstr::ENC::ASCII);
						m_year.trim();
					}
					if (m_comment.size() == 0) {
						m_comment = hxlstr((uint8_t*)id3v1Hdr.comment, sizeof(id3v1Hdr.comment), hxlstr::ENC::ASCII);
						m_comment.trim();
					}
					if (m_trackNo.size() == 0) {
						m_trackNo = hxlstr((uint8_t*)id3v1Hdr.track, sizeof(id3v1Hdr.track), hxlstr::ENC::ASCII);
						m_trackNo.trim();
					}
					if (m_genre.size() == 0) {
						m_genre = genre_table[id3v1Hdr.genre];
					}
				}
				else {
					//m_status = STATUS::MP3TAG_ILLFORMED_ID3V1;
				}
			} //if ID3V1.1
			else {
				m_status = STATUS::MP3TAG_NO_ID3V1;	//neither id3v1 nor id3v2 
			}
		}//if mp3
		else if (isWma(filePath) == true) {
			m_status = STATUS::MP3TAG_NOT_MP3;
		}
	}// file open
	else {
		std::cout << "Mp3 cannot be opened" << std::endl;
	}

}


Mp3Tag::~Mp3Tag() {

}


ID3V2FRM* Mp3Tag::getNextFrame(ID3V2FRM* currentFrame) {

	ID3V2FRM* nextFrame = nullptr;
	if (currentFrame != nullptr) {

		int payloadSize = calcID3FieldSize(currentFrame->payloadSize);

		nextFrame = (ID3V2FRM*)((int)currentFrame + 10 + payloadSize);

		int nextPayloadSize = calcID3FieldSize(nextFrame->payloadSize);		
	}

	return nextFrame;
}

bool Mp3Tag::getID3Header(std::filesystem::path filePath, ID3V2HDR* hdrv2, ID3V1HDR* hdrv1) {
	bool retVal = false;
	std::ifstream mp3File;
	mp3File.open(filePath, std::ios::binary);
	if (mp3File.is_open()) {

		mp3File.seekg(std::ios::beg);
		mp3File.read((char*)hdrv2, sizeof(ID3V2HDR));	//only read header first for effectiveness		
		mp3File.seekg(ID3V1_TAG_OFFSET, std::ios::end);
		mp3File.read((char*)hdrv1, sizeof(ID3V1HDR));	//only read header first for effectiveness		
		retVal = true;
		mp3File.close();
	}
	else {
		retVal = false;
	}
	return retVal;
}


void Mp3Tag::iterateFrames() {

	ID3V2FRM* p_id3v2NextFrm = nullptr;
	ID3V2FRM* p_id3v2CurrFrm = m_firstFrame;

	if (m_size > 0) {

		while (isIdValid(p_id3v2CurrFrm)) {
			Mp3Frame curFrame = Mp3Frame(p_id3v2CurrFrm);

			if (curFrame.id() == "TYER") {
				m_year = curFrame.text();
			}
			else if (curFrame.id() == "TALB") {
				m_album = curFrame.text();
			}
			else if (curFrame.id() == "TIT2") {
				m_title = curFrame.text();
			}
			else if (curFrame.id() == "TRCK") {
				m_trackNo = curFrame.text();
			}
			else if (curFrame.id() == "TCON") {
				m_genre = curFrame.text();
			}
			else if (curFrame.id() == "TPE2") {
				m_artist1 = curFrame.text();
			}
			else if (curFrame.id() == "TPE1") {
				m_artist2 = curFrame.text();
			}

			p_id3v2CurrFrm = getNextFrame(p_id3v2CurrFrm);

		}	//while
	}//if size is non zero
}

bool isMp3(std::filesystem::path filePath) {

	hxlstr input_file_ext = filePath.extension().string().c_str();
	bool retVal = (input_file_ext == ".mp3") || (input_file_ext == ".MP3") || (input_file_ext == ".Mp3");
	return retVal;
}

bool isWma(std::filesystem::path filePath) {
	hxlstr input_file_ext = filePath.extension().string().c_str();
	bool retVal = (input_file_ext == ".wma") || (input_file_ext == ".WMA") || (input_file_ext == ".Wma");
	return retVal;
}