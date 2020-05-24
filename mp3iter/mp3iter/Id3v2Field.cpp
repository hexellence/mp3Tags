#include "Id3v2Field.h"


int Id3v2Field::calcID3v2SizeField(const uint8_t* number, bool nonStandardCalc) {
	
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

bool Id3v2Field::isID3v2FieldIdValid(const FrmHdr* hdr) {
	bool idFound = false;
	if (hdr != nullptr) {
		for (auto item : ID3_FrameDefs) {
			if ((hdr->id[0] == item.sLongTextID[0]) && (hdr->id[1] == item.sLongTextID[1])
				&& (hdr->id[2] == item.sLongTextID[2]) && (hdr->id[3] == item.sLongTextID[3])) {
				idFound = true;
				break;
			}
			else {
			}
		}
	}
	else {
		//std::cout << "isIdValid: illegal frame address" << std::endl;
	}
	return idFound;
}


bool Id3v2Field::isID3v2FieldIdValid(const TagHdr* hdr)
{
	bool isValid = false;
	if (hdr != nullptr) {
		if ((hdr->id[0] == 'I') && (hdr->id[1] == 'D') && (hdr->id[2] == '3'))
		{
			isValid = true;
		}
	}
	return isValid;
}