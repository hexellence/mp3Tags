#pragma once
#include "FrmHdr.h"
#include "Hdr.h"

class TagHdr : public Hdr
{
private:
	char _id[3];
	uint8_t _ver[2];
	uint8_t _flags;
	uint8_t _size[4];
	char _firstFrm;

public:
	//Don't want any auto constructors as the memory will be provided 
	//by reserving a memory and all functions work on that memory area
	TagHdr() = delete;			
	TagHdr(const TagHdr& other) = delete;

	//checkers
	bool valid() const
	{
		return (id() == "ID3");
	}


	//getters
	hxlstr id() const
	{
		return hxlstr(_id, 3);
	}	


	int size() const
	{
		return calcID3v2SizeField(_size);
	}


	FrmHdr* first()
	{
		FrmHdr* firstFrm = (FrmHdr*)&_firstFrm;		
		return firstFrm;
	}


	//setters
	void size(int size)
	{
		int temp = size;
		int i = 3;
		while (temp > 0) {

			_size[i] = temp % 128;
			i--;
			temp = temp / 128;
		}		
	}

	void id() 
	{
		_id[0] = 'I';
		_id[0] = 'D';
		_id[0] = '3';		
	}
};

