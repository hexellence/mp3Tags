#include <cassert>
#include "fileUtil.h"
#include "Id3v2Tag.h"

Id3v2Tag::Id3v2Tag(std::filesystem::path filePath) : m_pFirstFrame(nullptr), m_pWholeTag(nullptr), m_pTagHdr(nullptr) {

	TagHdr hdr;
	int size = getID3v2TagHeader(filePath, &hdr);
	if (size > 0)
	{
		m_pWholeTag = new char[size * 2];
		m_pFirstFrame = (FrmHdr*)readID3v2Tag(filePath, m_pWholeTag, size);
		if (m_pFirstFrame != nullptr)
		{
			m_pTagHdr = (TagHdr*)m_pWholeTag;
		}
	}
}


int Id3v2Tag::getID3v2TagHeader(std::filesystem::path filePath, TagHdr* hdr)
{
	int tagSize = 0;
	if (readFile(filePath, (char*)hdr, sizeof(*hdr)))
	{
		if (hdr->isValid())
		{
			tagSize = hdr->size();
		}
	}
	return tagSize;
}


char* Id3v2Tag::readID3v2Tag(std::filesystem::path filePath, char* id3v2Tag, int tagsize) {
	FrmHdr* firstFrame = nullptr;

	if (checkID3v2TagExist(filePath))
	{
		if (readFile(filePath, id3v2Tag, tagsize))
		{
			firstFrame = (FrmHdr*)((int)id3v2Tag + ID3V2_HDR_SIZE);
			if (firstFrame->isValid() != true)
			{
				firstFrame = nullptr;
			}
		}//read success			
	}//if there is ID3 Tag

	return (char*)firstFrame;
}


bool Id3v2Tag::checkID3v2TagExist(std::filesystem::path filePath)
{
	bool retVal = false;

	TagHdr hdr;
	retVal = readFile(filePath, (char*)&hdr, sizeof(hdr));
	retVal = retVal && hdr.isValid();

	return retVal;
}

//Id3v2Tag::Id3v2Tag() : m_pos(0), m_tagSize(0), m_pFirstFrame(nullptr), m_pWholeTag(nullptr), m_pTagHdr(nullptr){
//	
//	std::cout << "Default Constructor" << std::endl;
//}




Id3v2Tag::~Id3v2Tag() {
	delete[] m_pWholeTag;
}


Id3v2Tag::FrmHdr& Id3v2Tag::operator[](const char* id)
{
	Id3v2Tag::iterator it = find(id);
	return (*it);
}


Id3v2Tag::iterator Id3v2Tag::find(const char* id) {

	Id3v2Tag::iterator tempVal = end();

	for (Id3v2Tag::iterator it = begin(); it != end(); it++)
	{
		if (((*it)._id[0] == id[0]) && ((*it)._id[1] == id[1]) && ((*it)._id[2] == id[2]) && ((*it)._id[3] == id[3])) {
			tempVal = it;
			break;
		}
	}
	return tempVal;
}


Id3v2Tag::FrmHdr& Id3v2Tag::fetch(FrmHdr* hdr)
{
	return *hdr;
}


Id3v2Tag::FrmHdr* Id3v2Tag::FrmHdr::nextFrame() {

	FrmHdr* nextFrame = nullptr;
	FrmHdr* currFrame = (FrmHdr*)_id;

	int plSize = calcID3v2SizeField(_payloadSize);

	nextFrame = (FrmHdr*)((int)currFrame + ID3V2_HDR_SIZE + plSize);

	if (!nextFrame->isValid())
	{
		plSize = calcID3v2SizeField(_payloadSize, true); //non standard 
		nextFrame = (FrmHdr*)((int)currFrame + ID3V2_HDR_SIZE + plSize);

		if (!nextFrame->isValid())
		{
			nextFrame = nullptr;
		}
	}
	return nextFrame;
}


Id3v2Tag::FrmHdr& Id3v2Tag::FrmHdr::operator=(const char* pl)
{
	TagHdr* tag = (TagHdr*)_pTag;
	FrmHdr* curFrame = (FrmHdr*)_id;
	FrmHdr* nexFrame = curFrame->nextFrame();	
	int size = tag->size();

	char* lastaddress = (char*)((int)tag + tag->size());
	int copySize = (int)lastaddress - (int)nexFrame;
	
	//set payload size
	uint32_t sizeNewPayload = hxl::length(pl) + 1;
	char* copyDest = (char*)((int)curFrame + sizeNewPayload + 10);	

	memcpy(copyDest, nexFrame, copySize);
	
	memcpy(_payloadSize, &sizeNewPayload, 4);
	//convert endianness
	convertEndianness(_payloadSize, 4);
	char* pltext = &_firstChar;
	pltext[0] = 0x00;	//ASCII
	memcpy(&pltext[1], pl, size - 1);


	return *this;
}