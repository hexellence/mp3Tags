#include <iostream>
#include "ID3V2Tag.h"



const uint8_t* getTextAddress(const ID3V2FRM* frame) {
	const uint8_t* txtAddress = nullptr;
	int rawAddress = (int)&frame->payload.txt;

	switch (frame->payload.enc) {
	case ENC_ASCII:
		txtAddress = (const uint8_t*)(rawAddress);	//no BOM to take into account
		break;
	case ENC_UTF16LEWBOM:
		txtAddress = (const uint8_t*)(rawAddress + 2);	//no BOM to take into account
		break;
	default:
		txtAddress = nullptr;
		break;
	}
	return txtAddress;
}


int calcID3FieldSize(const uint8_t* number) {
	int retVal = 0;
	retVal = number[3] +
		number[2] * (1 << 8) +
		number[1] * (1 << 16) +
		number[0] * (1 << 24);
	return retVal;
}


int GetID3v2PayloadSize(const ID3V2FRM* frame) {
	int payloadSize = 0;
	if (frame != nullptr) {
		payloadSize = calcID3FieldSize(frame->payloadSize);
		if (payloadSize > 0) {
			switch (frame->payload.enc) {
			case ENC_ASCII:
				payloadSize = payloadSize - 1;	//disregard the type indicator byte
				break;
			case ENC_UTF16LEWBOM:
				payloadSize = payloadSize - 3;	//disregard the type indicator byte and BOM
				break;
			default:
				payloadSize = 0;
				break;
			}
		}
		else {
			payloadSize = 0;
		}
	}
	return payloadSize;
}


bool isIdValid(const ID3V2FRM* frame) {
	bool idFound = false;
	if (frame != nullptr) {
		for (auto item : ID3_FrameDefs) {
			if ((frame->id[0] == item.sLongTextID[0]) && (frame->id[1] == item.sLongTextID[1])
				&& (frame->id[2] == item.sLongTextID[2]) && (frame->id[3] == item.sLongTextID[3])) {
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