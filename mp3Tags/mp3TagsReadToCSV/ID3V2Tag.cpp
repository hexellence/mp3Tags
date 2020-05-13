#include <filesystem>
#include <fstream>
#include <iostream>
#include "ID3V2Tag.h"




void setID3v2Header(std::filesystem::path filePath, char* id3Tag, int tagSize)
{
	long long sourceSize = getFileSize(filePath);

	std::filesystem::path tempFilePath = filePath.parent_path();
	std::filesystem::path tempFileName = "/HXL_"; 
		
	tempFileName += filePath.filename();
	tempFilePath += tempFileName;	

	std::ofstream tempFile;
	tempFile.open(tempFilePath, std::ios::binary);

	std::ifstream mp3File;
	mp3File.open(filePath, std::ios::binary);
		
	long long offset = getAudioOffset(mp3File);
	int audioSize = sourceSize - offset;

	char* audioContent = new char[audioSize];
	mp3File.seekg(offset, std::ios::beg);
	mp3File.read(audioContent, audioSize);
	
	tempFile.write(id3Tag, tagSize);	//first write the tag
	tempFile.write(audioContent, audioSize);	
	delete[] audioContent;
	mp3File.close();
	tempFile.close();
}


std::ifstream::pos_type getFileSize(std::filesystem::path filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

int getAudioOffset(std::filesystem::path filePath) 
{
	std::ifstream mp3File;
	mp3File.open(filePath, std::ios::binary);
	int offset = getAudioOffset(mp3File);
	mp3File.close();
	return offset;
}

int getAudioOffset(std::ifstream& mp3File)
{
	int offset = 0;	
	
	if (mp3File.is_open()) {
		uint8_t bytes[2]{};

		while ((bytes[0] != mp3AudioHeader[0]) || (bytes[1] != mp3AudioHeader[1])) {
			mp3File.read((char*)&bytes[0], 1);	//only read header first for effectiveness
			offset++;
			if (bytes[0] == mp3AudioHeader[0]) {
				mp3File.read((char*)&bytes[1], 1);	//only read header first for effectiveness				
				offset++;
			}
		}
	}
	if (offset > 3) {
		offset = offset - 2;
	}
	return offset;
}

bool getID3v2Header(std::filesystem::path filePath, ID3V2HDR* hdr) {
	bool retVal = false;
	std::ifstream mp3File;
	
	mp3File.open(filePath, std::ios::binary);
	if (mp3File.is_open()) {

		mp3File.seekg(std::ios::beg);
		mp3File.read((char*)hdr, sizeof(ID3V2HDR));	//only read header first for effectiveness				
		retVal = true;
		mp3File.close();
	}
	else {
		retVal = false;
	}
	return retVal;
}


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

ID3V2FRM* getNextFrame(ID3V2FRM* currentFrame) {

	ID3V2FRM* nextFrame = nullptr;
	if (currentFrame != nullptr) {

		int payloadSize = calcID3FieldSize(currentFrame->payloadSize);

		nextFrame = (ID3V2FRM*)((int)currentFrame + 10 + payloadSize);
	}

	return nextFrame;
}