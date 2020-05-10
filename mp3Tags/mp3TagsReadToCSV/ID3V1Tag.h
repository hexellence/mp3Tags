#pragma once

#include <cstdint>


static const int ID3V1_FIELD_SIZE = 30;
static const int ID3V1_TAG_SIZE = 128;
static const int ID3V1_TAG_OFFSET = (0 - ID3V1_TAG_SIZE);

#pragma pack(push, 1)

struct ID3V1HDR {
	char tag[3];
	char title[ID3V1_FIELD_SIZE];
	char artist[ID3V1_FIELD_SIZE];
	char album[ID3V1_FIELD_SIZE];
	char year[4];
	char comment[28];
	uint8_t zero;	//If a track number is stored, this byte contains a binary 0
	uint8_t track[2];	//The number of the track on the album, or 0. Invalid, if previous byte is not a binary 0.
	uint8_t genre;	//enumeration
}; //Strings are either space or zero padded. ID3v1 is 128 bytes long


#pragma pack(pop)

