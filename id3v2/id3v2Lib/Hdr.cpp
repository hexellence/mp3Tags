#include "Hdr.h"

/*
	calcID3v2SizeField() returns the size by reading the size field. 
	the field normally must comply to the standard and leave the most significant bit of each byte 0; 
	however sometimes I observed some mp3 files don't comply its been seen as first frame address regardless there is a valid frame
*/
int Hdr::calcID3v2SizeField(const uint8_t* number, bool nonStandardCalc) {

	int retVal = 0;

	if ((number[3] < 127) && (number[2] < 127) && (number[1] < 127) && (number[0] < 127) && !nonStandardCalc)
	{
		retVal = number[3] +
			number[2] * (1 << 7) +
			number[1] * (1 << 14) +
			number[0] * (1 << 21);
	}
	else {
		// Normally we should not get the following but as it appears some tracks have the payload size normally
		retVal = number[3] +
			number[2] * (1 << 8) +
			number[1] * (1 << 16) +
			number[0] * (1 << 24);
	}
	if (retVal < 0) 
	{
		retVal = 0;
	}
	return retVal;
}


/*
	hdr_size() method returns the size of the header.
*/
int Hdr::hdr_size()
{
	return ID3V2_HDR_SIZE;
}



