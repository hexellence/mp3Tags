#include <filesystem>
#include <fstream>
#include <iostream>
#include "ID3V2Tag.h"
#include "ID3V1Tag.h"
#include "filesys.h"

void setID3Headers(std::filesystem::path filePath, char16_t* newFileName, const char* id3v2Tag, const ID3V1HDR* id3v1Tag, int id3v2TagSize)
{
	int sourceSize = (int)getFileSize(filePath);

	std::filesystem::path outFilePath = filePath.parent_path();
	std::filesystem::path outFileName = "/";
	outFileName += newFileName;
	outFilePath += outFileName;

	std::ofstream outFile;
	std::ifstream mp3File;

	bool isID3v1Tag = isID3v1(filePath);


	int offset = getAudioOffset(filePath);


	int audioSize = sourceSize - offset;

	if (isID3v1Tag) {
		audioSize -= 128;	//removes the old style tagging
	}

	if (audioSize > 0) 
	{
		char* audioContent = new char[audioSize];
		mp3File.open(filePath, std::ios::binary | std::ios::in);
		if (mp3File.is_open()) 
		{			
			mp3File.seekg(offset, std::ios::beg);
			mp3File.read(audioContent, audioSize);
			mp3File.close();
		}

		outFile.open(outFilePath, std::ios::binary);
		if (outFile.is_open())
		{
			outFile.write(id3v2Tag, id3v2TagSize);	//first write the tag
			outFile.write(audioContent, audioSize);	//place the audio
			outFile.write((const char*)id3v1Tag, 128);	//add the old style tagging
			outFile.close();	
		}
		delete[] audioContent;
	}
	else {
		std::cout << "file audio detection failed" << std::endl;
	}
}


int getAudioOffset(std::filesystem::path filePath)
{
	std::ifstream mp3File;
	int fileSize = (int)getFileSize(filePath);
	mp3File.open(filePath, std::ios::binary);
	int offset = getAudioOffset(mp3File, fileSize);
	mp3File.close();
	return offset;
}

int getAudioOffset(std::ifstream& mp3File, int size)
{
	int offset = 0;

	if (mp3File.is_open()) {
		uint8_t bytes[2]{};

		while (((bytes[0] != mp3AudioHeader[0]) || (bytes[1] != mp3AudioHeader[1])) && (offset < size)) {
			mp3File.read((char*)&bytes[0], 1);	//only read header first for effectiveness
			offset++;
			if (bytes[0] == mp3AudioHeader[0]) {
				mp3File.read((char*)&bytes[1], 1);	//only read header first for effectiveness				
				offset++;
			}
		}
	}
	if (offset >=2) {
		offset = offset - 2;
	}
	return offset;
}

bool checkifID3V2TagExist(std::filesystem::path filePath)
{
	char id[4]{};
	bool retVal = false;

	retVal = readFile(filePath, id, 3);
	if ((id[0] == 'I') && (id[1] == 'D') && (id[2] == '3') && retVal)
	{
		retVal = true;
	}
	else
	{
		retVal = false;
	}
	return retVal;
}

bool getID3v2Header(std::filesystem::path filePath, ID3V2HDR* hdr) 
{
	bool retVal = readFile(filePath, (char*)hdr, sizeof(ID3V2HDR));		
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


int calcID3FieldSize(const uint8_t* number, bool nonStandardCalc) {
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
		
		if(!isIdValid(nextFrame))
		{
			payloadSize = calcID3FieldSize(currentFrame->payloadSize, true); //non standard 
			nextFrame = (ID3V2FRM*)((int)currentFrame + 10 + payloadSize);
			if (!isIdValid(nextFrame))
			{
				nextFrame = nullptr;
			}
		}		
	}

	return nextFrame;
}


int getFrmSize(ID3V2FRM* frm)
{
	int size = 0;
	if (frm != nullptr) {
		if (isIdValid(frm))
		{
			int payloadSize = calcID3FieldSize(frm->payloadSize);
			size = 10 + payloadSize;
		}
	}

	return size;
}

ID3V2FRM* getTag(std::filesystem::path filePath, char* id3Tag) {
	ID3V2FRM* retVal = nullptr;	

	if (isMp3(filePath)) 
	{	
		if (checkifID3V2TagExist(filePath))
		{
			ID3V2HDR hdr{};
			if (getID3v2Header(filePath, &hdr))
			{
				int tagsize = getTagSize(&hdr);
				if (readFile(filePath, id3Tag, tagsize))
				{
					if (isIdValid(&(((ID3V2HDR*)id3Tag)->firstFrame)))
					{
						retVal = &(((ID3V2HDR*)id3Tag)->firstFrame);
					}
				}//read success
			}//get success
		}//if there is ID3 Tag
	}//is MP3 file
	
	return retVal;
}

int getTagSize(ID3V2HDR* hdr)
{
	return calcID3FieldSize(hdr->size) + 10;
}

void setTagSize(ID3V2HDR* hdr, int size)
{
	int temp = size;
	int i = 3;
	while (temp > 0) {

		hdr->size[i] = temp % 128;
		i--;
		temp = temp / 128;
	}	
}