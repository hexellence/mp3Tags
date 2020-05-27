#pragma once

#include <cstdint>
#include "Id3v2Defs.h"
#include "Hdr.h"
#include "hxlstr.h"

using namespace hxl;

class FrmHdr : public Hdr
{
private:
	char _id[4];
	uint8_t _payloadSize[4];
	uint8_t _flags[2];
	uint8_t _payload;

public:
	FrmHdr() = delete;

	FrmHdr(hxlstr id, hxlstr text) 
	{
		this->id(id);
		this->content(text);
		if (text.enc() == hxlstr::ENC::ASCII)
		{
			this->size(text.size() + 1);
		}
		else if (text.enc() == hxlstr::ENC::UTF16LE)
		{
			this->size(text.size() + 3);
		}
		_flags[0] = 0x00;
		_flags[1] = 0x00;
	}

	FrmHdr(const FrmHdr& other) = delete;
	
	
	bool valid()
	{
		bool idFound = false;
		for (auto item : ID3_FrameDefs) {
			if(this->id() == item.sLongTextID)			
			{
				idFound = true;
				break;
			}
		}
		return idFound;
	}

	//getters
	hxlstr id() 
	{
		return hxlstr(_id, 4);
	}


	int size() 
	{
		
		FrmHdr* nextFrame = nullptr;

		int plSize = calcID3v2SizeField(_payloadSize);
		int retVal = plSize;
		nextFrame = (FrmHdr*)((int)this + ID3V2_HDR_SIZE + plSize);

		if (!nextFrame->valid())
		{		
			plSize = calcID3v2SizeField(_payloadSize, true); //non standard 
			nextFrame = (FrmHdr*)((int)this + ID3V2_HDR_SIZE + plSize);

			if (nextFrame->valid())
			{
				retVal = plSize;
			}
		}
		return retVal;
	}

	FrmHdr* next()
	{		
		FrmHdr* nextFrame = nullptr;

		int plSize = calcID3v2SizeField(_payloadSize);
		nextFrame = (FrmHdr*)((int)this + ID3V2_HDR_SIZE + plSize);

		if (!nextFrame->valid())
		{
			plSize = calcID3v2SizeField(_payloadSize, true); //non standard 
			nextFrame = (FrmHdr*)((int)this + ID3V2_HDR_SIZE + plSize);

			if (!nextFrame->valid())
			{
				nextFrame = nullptr;
			}
		}
		return nextFrame;
	}

	hxlstr content() 
	{
		hxlstr tempVal;
		char* content = (char*)&_payload;

		if (content[0] == 0x00) 
		{
			//ASCII
			if (this->size() > 1)
			{
				tempVal = hxlstr(&content[1], this->size() - 1);
			}
		}
		else if(content[0] == 0x01)
		{
			//UNICODE
			if (this->size() > 4)
			{
				tempVal = hxlstr((uint8_t*)&content[3], this->size() - 3, hxlstr::ENC::UTF16LE);
			}
		}
		return tempVal;
	}



	//setters
	void id(hxlstr id) 
	{
		memcpy(_id, id.c_str(), 4);
	}


	void size(int size) 
	{
		int temp = size;
		int i = 3;
		while (temp > 0) {

			_payloadSize[i] = temp % 128;
			i--;
			temp = temp / 128;
		}
	}


	void content(hxlstr content) 
	{
		hxlstr tempVal;
		uint8_t* pld = &_payload;

		if (content.enc() == hxlstr::ENC::ASCII)
		{
			//ASCII
			pld[0] = 0x00;
			memcpy(&pld[1], content.c_str(), content.size());		
			this->size(content.size() + 1);
		}
		else if (content.enc() == hxlstr::ENC::UTF16LE)
		{
			//UNICODE
			pld[0] = 0x01;
			pld[1] = 0xFE;
			pld[2] = 0xFF;
			memcpy(&pld[3], content.c16_str(), content.size());
			this->size(content.size() + 3);
		}		
	}
};

