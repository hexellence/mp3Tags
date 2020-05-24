#include <cassert>
#include "fileUtil.h"
#include "Id3v2Tag.h"



int Id3v2Tag::getID3v2TagHeader(std::filesystem::path filePath, Id3v2Field::TagHdr* hdr)
{
	int tagSize = 0;
	if (readFile(filePath, (char*)hdr, sizeof(*hdr)))
	{
		if (isID3v2FieldIdValid(hdr))
		{
			tagSize = readID3v2FieldSize((const char*)hdr);
		}
	}
	return tagSize;
}

int Id3v2Tag::readID3v2FieldSize(const char* tag) {

	return calcID3v2SizeField(((Id3v2Field::TagHdr*)tag)->size) + ID3V2_HDR_SIZE;
}


char* Id3v2Tag::readID3v2Tag(std::filesystem::path filePath, char* id3v2Tag, int tagsize) {
	char* firstFrameAddress = nullptr;

	if (checkID3v2TagExist(filePath))
	{
		if (readFile(filePath, id3v2Tag, tagsize))
		{
			firstFrameAddress = (char*)((int)id3v2Tag + ID3V2_HDR_SIZE);
			if (isID3v2FieldIdValid((FrmHdr*)firstFrameAddress) != true)
			{
				firstFrameAddress = nullptr;
			}
		}//read success			
	}//if there is ID3 Tag

	return firstFrameAddress;
}


bool Id3v2Tag::checkID3v2TagExist(std::filesystem::path filePath)
{
	bool retVal = false;

	Id3v2Field::TagHdr hdr;
	retVal = readFile(filePath, (char*)&hdr, sizeof(hdr));
	retVal = retVal && isID3v2FieldIdValid(&hdr);

	return retVal;
}

//Id3v2Tag::Id3v2Tag() : m_pos(0), m_tagSize(0), m_pFirstFrame(nullptr), m_pWholeTag(nullptr), m_pTagHdr(nullptr){
//	
//	std::cout << "Default Constructor" << std::endl;
//}

Id3v2Tag::Id3v2Tag(std::filesystem::path filePath) : m_pos(0), m_tagSize(0), m_pFirstFrame(nullptr), m_pWholeTag(nullptr), m_pTagHdr(nullptr), m_CurrentFrame(nullptr){

	Id3v2Field::TagHdr hdr;
	int size = getID3v2TagHeader(filePath, &hdr);
	if (size > 0)
	{
		m_pWholeTag = new char[size];
		m_pFirstFrame = (FrmHdr*)readID3v2Tag(filePath, m_pWholeTag, size);
		if (m_pFirstFrame != nullptr)
		{
			m_pTagHdr = (TagHdr*)m_pWholeTag;
			m_tagSize = size;
		}
	}
}


Id3v2Tag::~Id3v2Tag() {
	delete[] m_pWholeTag;
}


hxlstr& Id3v2Tag::operator[](hxlstr id) 
{
	Id3v2Tag::iterator it = find(id);
	return (*it).getFrameText();
}


Id3v2Tag::iterator Id3v2Tag::find(hxlstr id) {
	
	Id3v2Tag::iterator tempVal = end();	

	for (Id3v2Tag::iterator it = begin(); it != end(); it++)
	{
		if ((*it).getFrameId() == id) {
			tempVal = it;
			break;
		}
	}
	return tempVal;
}


Id3v2Frm& Id3v2Tag::fetch(FrmHdr* hdr)
{
	m_CurrentFrame = Id3v2Frm(hdr);
	return m_CurrentFrame;
}

Id3v2Field::FrmHdr* Id3v2Tag::iterator::nextFrame(Id3v2Field::FrmHdr* currentFrame) {
	
	Id3v2Field::FrmHdr* nextFrame = nullptr;
	if (currentFrame != nullptr) {

		int payloadSize = calcID3v2SizeField(currentFrame->payloadSize);

		nextFrame = (FrmHdr*)((int)currentFrame + ID3V2_HDR_SIZE + payloadSize);

		if (!isID3v2FieldIdValid(nextFrame))
		{
			payloadSize = calcID3v2SizeField(currentFrame->payloadSize, true); //non standard 
			nextFrame = (FrmHdr*)((int)currentFrame + ID3V2_HDR_SIZE + payloadSize);
			if (!isID3v2FieldIdValid(nextFrame))
			{
				nextFrame = nullptr;
			}
		}
	}

	return nextFrame;
}


