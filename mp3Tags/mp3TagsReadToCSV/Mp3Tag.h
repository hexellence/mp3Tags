#pragma once
#include <fstream>
#include <filesystem>
#include "hxlstr.h"
#include "ID3V2Tag.h"
#include "ID3V1Tagg.h"
#include "Mp3Frame.h"


class Mp3Tag
{
public:

	enum class STATUS{
		MP3TAG_SUCCESS,
		MP3TAG_NOT_MP3,
		MP3TAG_ILLFORMED_ID3V1,
		MP3TAG_ILLFORMED_ID3V2,
		MP3TAG_NO_ID3V1
	};


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
	STATUS m_status;

private:
	int m_size;
	ID3V2FRM* m_firstFrame;	
	
	bool getID3Header(std::filesystem::path, ID3V2HDR* hdrv2, ID3V1HDR* hdrv1);
	ID3V2FRM* getNextFrame(ID3V2FRM* currentFrame);
	void iterateFrames();

};

bool isMp3(std::filesystem::path filePath);
bool isWma(std::filesystem::path filePath);

