#pragma once
#include <cstdint>

class Hdr
{
public:
	static const int ID3V2_HDR_SIZE = 10;
	static int calcID3v2SizeField(const uint8_t* number, bool nonStandardCalc = false);
	void convertEndianness(uint8_t* inout, int size);
};

