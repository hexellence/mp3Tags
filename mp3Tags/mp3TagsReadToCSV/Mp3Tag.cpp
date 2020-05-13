#include <fstream>
#include "Mp3Tag.h"
#include "ID3V1Genre.h"
#include "filesys.h"


Mp3Tag::Mp3Tag(std::filesystem::path filePath) :
	m_size(0),
	m_firstFrame(nullptr),
	m_status(STATUS::MP3TAG_SUCCESS) {

	ID3V2HDR id3v2Hdr;
	ID3V1HDR id3v1Hdr;

	std::ifstream mp3File;

	if (getID3Headers(filePath, &id3v2Hdr, &id3v1Hdr)) {

		m_fileName = filePath.filename().generic_u16string().c_str();
		m_filePath = filePath.generic_u16string().c_str();

		if (isMp3(filePath) == true) {

			std::cout << m_filePath << std::endl;
			getID3v2Data(filePath, id3v2Hdr);
			getID3v1Data(id3v1Hdr);
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


void Mp3Tag::getID3v1Data(ID3V1HDR& hdr) {

	hxlstr tag((const uint8_t*)hdr.tag, 3, hxlstr::ENC::ASCII);

	if (tag == "TAG") {
		if (m_title.size() == 0) {
			m_title = hxlstr((uint8_t*)hdr.title, sizeof(hdr.title), hxlstr::ENC::ASCII);
			m_title.trim();
		}
		if (m_album.size() == 0) {
			m_album = hxlstr((uint8_t*)hdr.album, sizeof(hdr.album), hxlstr::ENC::ASCII);
			m_album.trim();
		}
		if (m_artist1.size() == 0) {
			m_artist1 = hxlstr((uint8_t*)hdr.artist, sizeof(hdr.artist), hxlstr::ENC::ASCII);
			m_artist1.trim();
		}
		if (m_artist2.size() == 0) {
			m_artist2 = m_artist1;
		}
		if (m_year.size() == 0) {
			m_year = hxlstr((const uint8_t*)hdr.year, 4, hxlstr::ENC::ASCII);
			m_year.trim();
		}
		if (m_comment.size() == 0) {
			m_comment = hxlstr((uint8_t*)hdr.comment, sizeof(hdr.comment), hxlstr::ENC::ASCII);
			m_comment.trim();
		}
		if (m_trackNo.size() == 0) {
			m_trackNo = hxlstr((uint8_t*)hdr.track, sizeof(hdr.track), hxlstr::ENC::ASCII);
			m_trackNo.trim();
		}
		if (m_genre.size() == 0) {
			m_genre = genre_table[hdr.genre];
		}
	}
}

void Mp3Tag::getID3v2Data(std::filesystem::path filePath, ID3V2HDR& hdr) {

	hxlstr id((const uint8_t*)hdr.id, 3, hxlstr::ENC::ASCII);

	std::ifstream mp3File;

	if (id == "ID3") {
		m_size = calcID3FieldSize(hdr.size);
		if (m_size > 0) {

			uint8_t* tagBuffer = new uint8_t[m_size + 10];	//size in file is excluding the header
			mp3File.open(filePath, std::ios::binary);
			mp3File.seekg(0, std::ios::beg);
			mp3File.read((char*)tagBuffer, (long long)m_size + 10);
			mp3File.close();

			ID3V2HDR* p_id3v2Header = (ID3V2HDR*)tagBuffer;

			m_firstFrame = &p_id3v2Header->firstFrame;
			iterateFrames();
			delete[] tagBuffer;
		}
		else {
			std::cout << "ID3 size wrong for file: " << m_fileName << std::endl;
		}

		if (hdr.flags != 0x00) {
			m_size = 0;
			m_status = STATUS::MP3TAG_ILLFORMED_ID3V2;
		}
	}
}

int Mp3Tag::createID3Tag(char* id3Tag) {
	int tagsize = 0;
	
	char16_t deneme[] = u"Deneme";
	if (id3Tag != nullptr) {
		ID3V2HDR* p_id3V2Tag = (ID3V2HDR*)id3Tag;		
		memcpy(&p_id3V2Tag->id, "ID3", 3);
		tagsize = tagsize + sizeof(p_id3V2Tag->id);

		p_id3V2Tag->ver[0] = 0x03;
		p_id3V2Tag->ver[1] = 0x00;
		tagsize = tagsize + sizeof(p_id3V2Tag->ver);

		p_id3V2Tag->flags = 0x00;
		tagsize = tagsize + sizeof(p_id3V2Tag->flags);

		p_id3V2Tag->size[0] = 0;
		p_id3V2Tag->size[1] = 0;
		p_id3V2Tag->size[2] = 0;
		p_id3V2Tag->size[3] = 0;
		tagsize = tagsize + sizeof(p_id3V2Tag->size);

		ID3V2FRM* p_id3V2Frm = &p_id3V2Tag->firstFrame;

		memcpy(&p_id3V2Frm[0].id, "TALB", 4);
		tagsize = tagsize + sizeof(p_id3V2Frm->id);

		p_id3V2Frm[0].payloadSize[0] = 0;
		p_id3V2Frm[0].payloadSize[1] = 0;
		p_id3V2Frm[0].payloadSize[2] = 0;
		p_id3V2Frm[0].payloadSize[3] = 0;
		tagsize = tagsize + sizeof(p_id3V2Frm->payloadSize);

		p_id3V2Frm[0].flags[0] = 0x00;
		p_id3V2Frm[0].flags[1] = 0x00;
		tagsize = tagsize + sizeof(p_id3V2Frm->flags);

		//BOM prepare
		p_id3V2Frm[0].payload.enc = 0x01;
		tagsize = tagsize + sizeof(p_id3V2Frm->payload.enc);
		//BOM 
		char* p_BOM = &p_id3V2Frm[0].payload.txt;
		memcpy(p_BOM, BOM, 2);
		tagsize = tagsize + 2;

		// to ease coding have a pointer to the text
		char* p_Data = &p_BOM[2];

		// copy album name
		memcpy(p_Data, (char*)deneme, 12);
		tagsize = tagsize + 12;

		//set payload size
		p_id3V2Tag->firstFrame.payloadSize[3] = 12 + 3;//3 comes from the 3 byte BOM field

		//set the tag size
		p_id3V2Tag->size[3] = tagsize - 10;		
	}
	return tagsize;
}