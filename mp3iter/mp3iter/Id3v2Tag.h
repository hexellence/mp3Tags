#pragma once

#include <filesystem>
#include "hxlstr.h"
#include "Id3v2Field.h"

using namespace hxl;

class Id3v2Tag : public Id3v2Field
{
public:

	class FrmHdr {
	public:
		char _id[4];
		uint8_t _payloadSize[4];
		uint8_t _flags[2];
		char _firstChar;
		uint8_t* _pTag;

		FrmHdr() {
			memset(_id, 0, 4);
			memset(_payloadSize, 0, 4);
			memset(_flags, 0, 2);
			_firstChar = '\0';
			_pTag = nullptr;
		}

		FrmHdr& operator=(const char* pl);
		FrmHdr* nextFrame();
		
		int plsize() {
			return calcID3v2SizeField(_payloadSize);
		}

		int size() {
			return calcID3v2SizeField(_payloadSize) + ID3V2_HDR_SIZE;
		}

		bool isValid() {
			bool idFound = false;
			for (auto item : ID3_FrameDefs) {
				if ((_id[0] == item.sLongTextID[0]) && (_id[1] == item.sLongTextID[1])
					&& (_id[2] == item.sLongTextID[2]) && (_id[3] == item.sLongTextID[3])) 
				{
					idFound = true;
					break;
				}
			}
			return idFound;
		}
	};

	class TagHdr {
	public:
		char _id[3];
		uint8_t _ver[2];
		uint8_t _flags;
		uint8_t _size[4];
		FrmHdr _firstFrm;

		TagHdr() : _firstFrm() {
			memcpy(_id, "ID3", 3);
			memset(_ver, 0, 2);
			_flags = 0x00;
			memset(_size, 0, 4);			 
		}

		int size() {
			return calcID3v2SizeField(_size) + ID3V2_HDR_SIZE;
		}

		bool isValid()
		{
			bool isValid = false;
			if ((_id[0] == 'I') && (_id[1] == 'D') && (_id[2] == '3'))
			{
				isValid = true;
			}
			return isValid;
		}
	};

	class iterator {

	private:
		FrmHdr* m_pos;
		Id3v2Tag& m_theTag;

	public:
		iterator(FrmHdr* pos, Id3v2Tag& aTag) : m_pos(pos), m_theTag(aTag) {}

		iterator& operator++(int)
		{
			m_pos = m_pos->nextFrame();
			return *this;
		}

		FrmHdr& operator*()
		{
			return *m_pos;
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


	};


	FrmHdr& fetch(FrmHdr* hdr);
	FrmHdr& operator[](const char* id);

	iterator begin()
	{
		return iterator(m_pFirstFrame, *this);
	}

	iterator end()
	{
		return iterator(nullptr, *this);
	}

	iterator find(const char* id);

	Id3v2Tag(std::filesystem::path filePath);
	~Id3v2Tag();

private:

	//int m_pos;
	int m_tagSize;
	FrmHdr* m_pFirstFrame;
	char* m_pWholeTag;
	TagHdr* m_pTagHdr;

	int getID3v2TagHeader(std::filesystem::path filePath, TagHdr* hdr);	
	bool checkID3v2TagExist(std::filesystem::path filePath);
	char* readID3v2Tag(std::filesystem::path filePath, char* id3v2Tag, int tagsize);

};
