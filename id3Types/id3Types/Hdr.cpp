#include "Hdr.h"


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
	return retVal;
}


void Hdr::convertEndianness(uint8_t* inout, int size) {

	int loopCount = size / 2;
	for (int i = 0; i < loopCount; ++i) {
		uint8_t temp = inout[size - 1 - i];
		inout[size - 1 - i] = inout[i];
		inout[i] = temp;
	}
}