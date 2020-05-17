#include <fstream>
#include "Mp3Tag.h"
#include "ID3V1Genre.h"
#include "filesys.h"
#include "hxlstr.h"


Mp3Tag::Mp3Tag(std::filesystem::path filePath) :
	m_size(0),
	m_firstFrame(nullptr),
	m_status(STATUS::MP3TAG_SUCCESS) {

	ID3V2HDR id3v2Hdr;
	ID3V1HDR id3v1Hdr;

	std::ifstream mp3File;

	if (isMp3(filePath) == true) {

		if (getID3Headers(filePath, &id3v2Hdr, &id3v1Hdr)) {

			m_fileName = filePath.filename().generic_u16string().c_str();
			m_filePath = filePath.generic_u16string().c_str();

			std::cout << m_filePath << std::endl;
			getID3v2Data(filePath, id3v2Hdr);
			getID3v1Data(id3v1Hdr);
		}
		else
		{
			std::cout << "Error getting mp3 headers" << std::endl;
		}
	}//if mp3
	else if (isWma(filePath) == true) {
		m_status = STATUS::MP3TAG_NOT_MP3;
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
				m_year = curFrame;
			}
			else if (curFrame.id() == "TALB") {
				m_album = curFrame;
			}
			else if (curFrame.id() == "TIT2") {
				m_title = curFrame;
			}
			else if (curFrame.id() == "TRCK") {
				m_trackNo = curFrame;
			}
			else if (curFrame.id() == "TCON") {
				m_genre = curFrame;
			}
			else if (curFrame.id() == "TPE2") {
				m_artist1 = curFrame;
			}
			else if (curFrame.id() == "TPE1") {
				m_artist2 = curFrame;
			}
			else if ((curFrame.id() == "COMM") && (m_comment.m_str.size() == 0)) {
				m_comment = curFrame;
			}

			p_id3v2CurrFrm = getNextFrame(p_id3v2CurrFrm);

		}	//while
	}//if size is non zero
}


void Mp3Tag::getID3v1Data(ID3V1HDR& hdr) {

	hxlstr tag((const uint8_t*)hdr.tag, 3, hxlstr::ENC::ASCII);

	if (tag == "TAG") {
		if (m_title.m_str.size() == 0) {
			m_title.m_str = hxlstr((uint8_t*)hdr.title, sizeof(hdr.title), hxlstr::ENC::ASCII);

			m_title.m_str.trim(u" \t\x0d\x0a");
			m_title.m_str.remove(u'\0');	//removes inbetween 
			m_title.m_str.remove(u'\t');

			m_title.m_id = "TIT2";
		}
		if (m_album.m_str.size() == 0) {
			m_album.m_str = hxlstr((uint8_t*)hdr.album, sizeof(hdr.album), hxlstr::ENC::ASCII);
			m_album.m_str.trim(u" \t\x0d\x0a");
			m_album.m_str.remove(u'\0');	//removes inbetween 
			m_album.m_str.remove(u'\t');
			m_album.m_id = "TALB";
		}
		if (m_artist1.m_str.size() == 0) {
			m_artist1.m_str = hxlstr((uint8_t*)hdr.artist, sizeof(hdr.artist), hxlstr::ENC::ASCII);
			m_artist1.m_str.trim(u" \t\x0d\x0a");
			m_artist1.m_str.remove(u'\0');	//removes inbetween 
			m_artist1.m_str.remove(u'\t');
			m_artist1.m_id = "TPE2";
		}
		if (m_artist2.m_str.size() == 0) {
			m_artist2 = m_artist1;
			m_artist2.m_id = "TPE1";
		}
		if (m_year.m_str.size() == 0) {
			m_year.m_str = hxlstr((const uint8_t*)hdr.year, 4, hxlstr::ENC::ASCII);
			m_year.m_str.trim(u" \t\x0d\x0a");
			m_year.m_str.remove(u'\0');	//removes inbetween 
			m_year.m_str.remove(u'\t');
			m_year.m_id = "TYER";
		}
		if (m_comment.m_str.size() == 0) {
			m_comment.m_str = hxlstr((uint8_t*)hdr.comment, sizeof(hdr.comment), hxlstr::ENC::ASCII);
			m_comment.m_str.trim(u" \t\x0d\x0a");
			m_comment.m_str.remove(u'\0');	//removes inbetween 
			m_comment.m_str.remove(u'\t');
			m_comment.m_id = "COMM";
		}
		if (m_trackNo.m_str.size() == 0) {
			m_trackNo.m_str = hxlstr((int)hdr.track);			
			m_trackNo.m_id = "TRCK";
		}
		if (m_genre.m_str.size() == 0) {
			m_genre.m_str = genre_table[hdr.genre];
			m_genre.m_str.trim(u" \t\x0d\x0a");
			m_genre.m_str.remove(u'\0');	//removes inbetween 
			m_genre.m_str.remove(u'\t');
			m_genre.m_id = "TCON";
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


int createID3v2Tag(char* id3TagOut, Mp3Frame* frames, int numOfFrames) {
	int tagsize = 0;

	if ((id3TagOut != nullptr) && (frames != nullptr)) {
		ID3V2HDR* p_id3V2Tag = (ID3V2HDR*)id3TagOut;
		memcpy(&p_id3V2Tag->id, "ID3", 3);
		tagsize = tagsize + sizeof(p_id3V2Tag->id);

		p_id3V2Tag->ver[0] = 0x03;
		p_id3V2Tag->ver[1] = 0x00;
		tagsize = tagsize + sizeof(p_id3V2Tag->ver);

		p_id3V2Tag->flags = 0x00;
		tagsize = tagsize + sizeof(p_id3V2Tag->flags);

		int p_id3V2NextFrm = (int)&p_id3V2Tag->firstFrame;

		for (int i = 0; i < numOfFrames; ++i) {

			int frameSize = createID3v2Frame(frames[i], (char*)p_id3V2NextFrm);
			p_id3V2NextFrm += frameSize;
			tagsize += frameSize;
		}
		//size field itself
		tagsize = tagsize + sizeof(p_id3V2Tag->size);

		uint32_t tagsizeToWrite = tagsize - 10; //the header is not included when writing
		setTagSize((ID3V2HDR*)p_id3V2Tag, tagsizeToWrite);
	}
	return tagsize;
}

int createID3v1Tag(ID3V1HDR* id3v1TagOut, Mp3Frame* frames, int numOfFrames) {
	int tagsize = 0;

	if ((id3v1TagOut != nullptr) && (frames != nullptr)) {
		ID3V1HDR* p_id3V1Tag = (ID3V1HDR*)id3v1TagOut;

		memcpy(&p_id3V1Tag->tag, "TAG", 3);
		p_id3V1Tag->zero = 0x00;
		for (int i = 0; i < numOfFrames; ++i)
		{
			if (frames[i].m_id == "TALB")
			{
				memset(p_id3V1Tag->album, 0, 30);
				hxlstrcopy((char*)p_id3V1Tag->album, frames[i].m_str, 30);
			}
			else if (frames[i].m_id == "TPE2")
			{
				memset(p_id3V1Tag->artist, 0, 30);
				hxlstrcopy((char*)p_id3V1Tag->artist, frames[i].m_str, 30);
			}
			else if (frames[i].m_id == "COMM")
			{
				memset(p_id3V1Tag->comment, 0, 28);
				hxlstrcopy((char*)p_id3V1Tag->comment, frames[i].m_str, 28);
			}
			else if (frames[i].m_id == "TIT2")
			{
				memset(p_id3V1Tag->title, 0, 30);
				hxlstrcopy((char*)p_id3V1Tag->title, frames[i].m_str, 30);
			}
			else if (frames[i].m_id == "TYER")
			{
				memset(p_id3V1Tag->year, 0, 4);
				hxlstrcopy((char*)p_id3V1Tag->year, frames[i].m_str, 4);
			}
			else if (frames[i].m_id == "TRCK")
			{
				char trackNo[2]{};
				hxlstrcopy(trackNo, frames[i].m_str, 2);				 
				p_id3V1Tag->track = atoi(trackNo);
			}
		}

		tagsize = 128;
	}
	return tagsize;
}