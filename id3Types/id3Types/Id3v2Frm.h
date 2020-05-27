#pragma once

#include "FrmHdr.h"
#include "TagHdr.h"
#include "FileUtil.h"
#include <filesystem>



class Id3v2Tag
{
private:

	TagHdr* m_pTagHdr;
	uint8_t* m_pWholeTag;


	int getID3v2TagHeader(std::filesystem::path filePath, TagHdr* hdr)
	{
		int tagSize = 0;
		if (readFile(filePath, (char*)hdr, sizeof(*hdr)))
		{
			if (hdr->valid())
			{
				tagSize = hdr->size();
			}
		}
		return tagSize;
	}


	FrmHdr* readID3v2Tag(std::filesystem::path filePath, uint8_t* id3v2Tag, int tagsize) {
		FrmHdr* firstFrame = nullptr;

		if (readFile(filePath, (char*)id3v2Tag, tagsize))
		{
			firstFrame = (FrmHdr*)((int)id3v2Tag + ID3V2_HDR_SIZE);
			if (firstFrame->valid() != true)
			{
				firstFrame = nullptr;
			}
		}//read success	
		return firstFrame;
	}

public:

	Id3v2Tag(std::filesystem::path filePath) {
		TagHdr hdr;
		int size = getID3v2TagHeader(filePath, &hdr);
		if (size > 0)
		{
			m_pWholeTag = new uint8_t[size * 2];
			FrmHdr* firstFrame = readID3v2Tag(filePath, m_pWholeTag, size + ID3V2_HDR_SIZE);
			if (firstFrame != nullptr)
			{
				m_pTagHdr = (TagHdr*)m_pWholeTag;
			}
		}
	}

	~Id3v2Tag() 
	{
		delete[] m_pWholeTag;
	}

	class iterator {

	private:
		FrmHdr* m_pos;
		Id3v2Tag& m_theTag;

	public:
		iterator(FrmHdr* pos, Id3v2Tag& aTag) : m_pos(pos), m_theTag(aTag) {}

		iterator& operator++(int)
		{
			m_pos = m_pos->next();
			return *this;
		}

		iterator& operator++()
		{
			m_pos = m_pos->next();
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
	};

	iterator begin()
	{
		return iterator(m_pTagHdr->first(), *this);
	}

	iterator end()
	{
		return iterator(nullptr, *this);
	}

	FrmHdr* find(hxlstr id)
	{
		FrmHdr* pFrm = m_pTagHdr->first();
		while (pFrm != nullptr)
		{
			if (pFrm->id() == id)
			{
				break;
			}

			pFrm = pFrm->next();
		}
		return pFrm;
	}

	FrmHdr* last()
	{
		FrmHdr* curFrm = m_pTagHdr->first();
		FrmHdr* nextFrm = curFrm;

		while (nextFrm != nullptr)
		{
			curFrm = nextFrm;
			nextFrm = curFrm->next();
		}
		return curFrm;
	}

	void del(hxlstr id)
	{
		FrmHdr* frmToDelete = find(id);
		int frameSizeDelete = frmToDelete->size() + ID3V2_HDR_SIZE;

		//overwrite old frame
		FrmHdr* nextFrame = frmToDelete->next();
		if (nextFrame == nullptr)
		{
			//if it was the last frame hypotetical nextFrame would be at this address
			nextFrame = (FrmHdr*)((int)frmToDelete + frameSizeDelete);
		}

		int moveSize = m_pTagHdr->size() + ID3V2_HDR_SIZE - (int)nextFrame + (int)m_pTagHdr;
		memcpy(frmToDelete, nextFrame, moveSize);

		
		//update Tag size		
		m_pTagHdr->size(m_pTagHdr->size() - frameSizeDelete);		
	}

	void append(hxlstr id, hxlstr text)
	{
		FrmHdr* lastFrm = last();
		int lastFrameSize = 0;
		if (lastFrm->valid())
		{
			lastFrameSize = lastFrm->size() + ID3V2_HDR_SIZE;
		}

		FrmHdr* newFrm = (FrmHdr*)((int)lastFrm + lastFrameSize);
		
		newFrm->id(id);
		newFrm->size(text.size());
		newFrm->flags();
		newFrm->content(text);		

		//update Tag size
		m_pTagHdr->size(m_pTagHdr->size() + newFrm->size() + ID3V2_HDR_SIZE);
	}

	void replace(hxlstr id, hxlstr text)
	{
		FrmHdr newfrm(id, text);
		del(id);
		append(id, text);
	}

};


/******************************************************************/

