#pragma once
#include <fstream>
#include <filesystem>
#include "hxlstr.h"
#include "ID3V2Tag.h"
#include "Mp3Frame.h"


class Mp3Tag
{
public:
	Mp3Tag() = delete;
	Mp3Tag(std::filesystem::path filePath);
	~Mp3Tag();	

	hxlstr m_title;
	hxlstr m_album;
	hxlstr m_artist1;
	hxlstr m_artist2;
	hxlstr m_year;
	hxlstr m_comment;
	hxlstr m_fileName;
	hxlstr m_filePath;
	hxlstr m_trackNo;
	hxlstr m_genre;
	int m_status;

private:
	int m_size;
	ID3V2FRM* m_firstFrame;
	ID3V2HDR* m_id3Header;
	
	bool getID3V2Header(std::ifstream& mp3File, ID3V2HDR* hdr);
	ID3V2FRM* getNextFrame(ID3V2FRM* currentFrame);
	void iterateFrames();

};

bool isMp3(std::filesystem::path filePath);
bool isWma(std::filesystem::path filePath);

