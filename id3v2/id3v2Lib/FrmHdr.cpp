#include "FrmHdr.h"

/*
	size(int) method lets the class set the payload size when a string is written to a frame.
	This function is not public, as it may cause problems if it is not used carefully.
*/
void FrmHdr::size(int size)
{
	int temp = size;
	int i = 3;
	while (temp > 0) {

		_payloadSize[i] = temp % 128;
		i--;
		temp = temp / 128;
	}
}

/*
	valid() method checks if the frame is valid.
*/
bool FrmHdr::valid() const
{
	hxlstr frmId = id();
	bool idFound = false;
	for (auto item : ID3_FrameDefs) {
		if (frmId == item.sLongTextID)
		{
			idFound = true;
			break;
		}
	}
	return idFound;
}

/*
	id() method returns the id of the frame.
*/
hxlstr FrmHdr::id() const
{
	return hxlstr(_id, 4);
}

/*
	size() method returns the size of the payload.
*/
int FrmHdr::size() const
{
	FrmHdr* nextFrame = nullptr;

	int plSize = calcID3v2SizeField(_payloadSize);
	int retVal = plSize;
	nextFrame = (FrmHdr*)((int)start() + hdr_size() + plSize);

	if (!nextFrame->valid())
	{
		plSize = calcID3v2SizeField(_payloadSize, true); //non standard 
		nextFrame = (FrmHdr*)((int)start() + hdr_size() + plSize);

		if (nextFrame->valid())
		{
			retVal = plSize;
		}
	}
	return retVal;
}


/*
	bytes() method returns the size of the payload and the header.
*/
int FrmHdr::bytes() const
{
	int retVal = size() + hdr_size();

	return retVal;
}

/*
	start() method returns the address of the header
*/
const FrmHdr* FrmHdr::start() const
{
	return (const FrmHdr*)_id;
}

/*
	next() method returns a pointer to the next frame. if the next frame is not valid it returns nullptr.
	it also checks non complying frames.
*/
FrmHdr* FrmHdr::next() const
{
	FrmHdr* nextFrame = nullptr;

	int plSize = calcID3v2SizeField(_payloadSize);
	nextFrame = (FrmHdr*)((int)start() + hdr_size() + plSize);

	if (!nextFrame->valid())
	{
		plSize = calcID3v2SizeField(_payloadSize, true); //non standard 
		nextFrame = (FrmHdr*)((int)start() + hdr_size() + plSize);

		if (!nextFrame->valid())
		{
			nextFrame = nullptr;
		}
	}
	return nextFrame;
}

/*
	value() method returns a copy of the frame data.	
*/
hxlstr FrmHdr::value() const
{
	hxlstr tempVal;
	char* content = (char*)&_payload;
	switch (content[0])
	{
	case 0x00:
		//ASCII
		if (size() > 1)
		{
			tempVal = hxlstr(&content[1], size() - 1);	
		}
		break;
	case 0x01:
		//UNICODE
		if (size() > 4)
		{
			tempVal = hxlstr((uint8_t*)&content[3], size() - 3, hxlstr::ENC::UNICD);
		}
		break;
	default:
		tempVal = "";
		break;
	}	
	tempVal.trim();
	tempVal.drop('\r');
	tempVal.drop('\t');
	tempVal.drop('\0');
	return tempVal;
}

/*
	id(hxlstr) method writes an id to the frame.
*/
void FrmHdr::id(hxlstr id)
{
	memcpy(_id, id.c_str(), 4);
}

/*
	flags() method clears the flags.
*/
void FrmHdr::flags()
{
	_flags[0] = 0x00;
	_flags[1] = 0x00;
}

/*
	value(hxlstr) method writes a value to the frame.
*/
void FrmHdr::value(hxlstr content)
{
	hxlstr tempVal;
	uint8_t* pld = &_payload;

	if (content.enc() == hxlstr::ENC::ASCII)
	{
		//ASCII
		pld[0] = 0x00;
		memcpy(&pld[1], content.c_str(), content.size());
		size(content.size() + 1);
	}
	else if (content.enc() == hxlstr::ENC::UNICD)
	{
		//UNICODE
		pld[0] = 0x01;
		pld[1] = 0xFF;
		pld[2] = 0xFE;
		memcpy(&pld[3], content.c16_str(), content.size());
		size(content.size() + 3);
	}
}

/*
	stream operator overload
*/
std::ostream& operator<<(std::ostream& out, const FrmHdr& frm) {
	out << frm.id() << ": " << frm.value() << std::flush;
	return out;
}