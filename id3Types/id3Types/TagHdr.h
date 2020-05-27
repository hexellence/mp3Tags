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
	FrmHdr _firstFrm;

public:
	//Constructors
	TagHdr() : _firstFrm(FrmHdr("", ""))
	{
		memcpy(_id, "ID3", 3);
		_flags = 0x00;
		memset(_ver, 0, 2);
		memset(_size, 0, 4);		
	}

	//checkers
	bool valid()
	{
		return ((_id[0] == 'I') && (_id[1] == 'D') && (_id[2] == '3'));
	}


	//getters
	hxlstr id()
	{
		return hxlstr(_id, 3);
	}	


	int size()
	{
		return calcID3v2SizeField(_size);
	}


	FrmHdr* first()
	{
		return &_firstFrm;
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
};

