#pragma once

#include <filesystem>
#include <iostream>
#include "TagHdr.h"
#include "FileUtil.h"

using namespace std;

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

		//getting the size first
		char* pTagHdr = new char[sizeof(TagHdr)];
		int size = getID3v2TagHeader(filePath, (TagHdr*)pTagHdr);
		delete[] pTagHdr;

		// if size is successful
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


	iterator find(hxlstr id)
	{
		iterator it = iterator(m_pTagHdr->first(), *this);
		while (it != end())
		{
			if ((*it).id() == id)
			{
				break;
			}
			it++;
		}
		return it;
	}


	iterator last()
	{
		iterator it = begin();
		iterator itLast = begin();
		while(it != end())
		{
			itLast = it;
			it++;
		}
		return itLast;
	}


	void del(hxlstr id)
	{
		iterator it = find(id);
		del(it);
	}


	void del(iterator it)
	{		
		if (it != end())
		{
			FrmHdr* frmToDelete = &(*it);
			//FrmHdr* frmToDelete = find(id);
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
	}


	void push_back(hxlstr id, hxlstr text)
	{
		if (checkId(id))
		{
			iterator it = last();
			FrmHdr* lastFrm = &(*it);

			int lastFrameSize = 0;
			if (lastFrm->valid())
			{
				lastFrameSize = lastFrm->size() + ID3V2_HDR_SIZE;
			}

			FrmHdr* newFrm = (FrmHdr*)((int)lastFrm + lastFrameSize);

			newFrm->id(id);
			newFrm->flags();
			newFrm->value(text);

			//update Tag size
			m_pTagHdr->size(m_pTagHdr->size() + newFrm->size() + ID3V2_HDR_SIZE);
		}
	}


	void insert(iterator it, hxlstr id, hxlstr text)
	{			
		if ((it != end()) && checkId(id))
		{
			int newFrmSize = (text.enc() == hxlstr::ENC::ASCII) ? text.size() + 1 : text.size() + 3;
			newFrmSize += ID3V2_HDR_SIZE;

			FrmHdr* presentFrm = &(*it);	//this frame will slide as much as our new frame size is	
			FrmHdr* newLocation = (FrmHdr*)((int)presentFrm + newFrmSize); //present frame's new location

			//calculate how much data will be moved
			int lastAddress = (int)m_pTagHdr + m_pTagHdr->size() + ID3V2_HDR_SIZE;
			int moveSize = lastAddress - (int)presentFrm;
			
			//copy and make space where the former frame was
			if (moveSize > 0)
			{
				memcpy(newLocation, presentFrm, moveSize);
			}
			else
			{
				cout << "sizeProblem" << endl;
			}
			
			//presentFrm will now contain new
			presentFrm->id(id);
			presentFrm->flags();			
			presentFrm->value(text);
			//update Tag size		
			m_pTagHdr->size(m_pTagHdr->size() + newFrmSize);
		}		
	}


	void clear()
	{
		iterator it = begin();

		while ((*it).valid())
		{
			del(it);
		}
	}
};

