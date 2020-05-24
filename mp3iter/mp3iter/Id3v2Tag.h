#pragma once

#include <filesystem>
#include "hxlstr.h"
#include "Id3v2Field.h"
#include "Id3v2Frm.h"

using namespace hxl;

class Id3v2Tag : public Id3v2Field
{
private:

	int m_pos;
	int m_tagSize;
	FrmHdr* m_pFirstFrame;
	char* m_pWholeTag;
	TagHdr* m_pTagHdr;	
	Id3v2Frm m_CurrentFrame;
	

	int getID3v2TagHeader(std::filesystem::path filePath, Id3v2Field::TagHdr* hdr);
	virtual int readID3v2FieldSize(const char* tag);
	bool checkID3v2TagExist(std::filesystem::path filePath);
	char* readID3v2Tag(std::filesystem::path filePath, char* id3v2Tag, int tagsize);	

public:
	
	class iterator {

	private:
		FrmHdr* m_pos;
		Id3v2Tag& m_theTag;	

	public:
		iterator(FrmHdr* pos, Id3v2Tag& aTag) : m_pos(pos), m_theTag(aTag) {}

		iterator& operator++(int)
		{
			m_pos = nextFrame(m_pos);
			return *this;
		}

		Id3v2Frm& operator*()
		{
			return m_theTag.fetch(m_pos);
		}

		bool operator!=(const iterator& other) const {
			return m_pos != other.m_pos;
		}		

		const iterator& operator=(const iterator& other) {
			m_pos = other.m_pos;
			m_theTag = other.m_theTag;
			return *this;
		}

	private:
		
		FrmHdr* nextFrame(FrmHdr* currentFrame);
	};

	Id3v2Frm& fetch(FrmHdr* hdr);	
	hxlstr& operator[](hxlstr id);

	iterator begin()
	{
		return iterator(m_pFirstFrame, *this);
	}

	iterator end()
	{
		return iterator(nullptr, *this);
	}	

	iterator find(hxlstr id);

	Id3v2Tag(std::filesystem::path filePath);
	~Id3v2Tag();
	
};
