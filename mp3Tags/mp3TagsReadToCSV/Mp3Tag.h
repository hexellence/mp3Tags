#pragma once
#include <fstream>
#include <filesystem>
#include "hxlstr.h"
#include "ID3.h"
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


	Mp3Frame m_title;
	Mp3Frame m_album;
	Mp3Frame m_artist1;
	Mp3Frame m_artist2;
	Mp3Frame m_year;
	Mp3Frame m_comment;
	hxlstr m_fileName;
	hxlstr m_filePath;
	Mp3Frame m_trackNo;
	Mp3Frame m_genre;
	STATUS m_status;

private:
	int m_size;
	ID3V2FRM* m_firstFrame;	
	void getID3v1Data(ID3V1HDR& hdr);
	void getID3v2Data(std::filesystem::path filePath, ID3V2HDR& hdr);	
	void iterateFrames();
	

};

int createID3v2Tag(char* id3v2TagOut, Mp3Frame* frames, int numOfFrames);
int createID3v1Tag(ID3V1HDR* id3v1TagOut, Mp3Frame* frames, int numOfFrames);

