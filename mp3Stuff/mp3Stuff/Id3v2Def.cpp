#include "Id3v2Def.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "fileutil.h"

void setID3Data(std::filesystem::path filePath, char16_t* newFileName, const char* id3v2Tag, const ID3V1HDR* id3v1Tag, int id3v2TagSize)
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
	if (offset >= 2) {
		offset = offset - 2;
	}
	return offset;
}


bool checkID3v2TagExist(std::filesystem::path filePath)
{
	bool retVal = false;
	if (isMp3(filePath)) {
		ID3V2TAGHDR tag;
		retVal = readFile(filePath, (char*)&tag, sizeof(tag));
		retVal = isID3v2TagIdValid(&tag);
	}

	return retVal;
}


int getID3v2TagHeader(std::filesystem::path filePath, ID3V2TAGHDR* hdr)
{
	int tagSize = 0;
	if (readFile(filePath, (char*)hdr, sizeof(ID3V2TAGHDR))) 
	{
		if (isID3v2TagIdValid(hdr))
		{
			tagSize = readID3v2TagSize(hdr);
		}
	}
	return tagSize;
}


const uint8_t* getID3v2PayloadTextAddress(const ID3V2FRMHDR* frame) {
	const uint8_t* payloadTextAddress = nullptr;
	int rawAddress = 0;
	if(frame != nullptr)
	{
		payloadTextAddress = (const uint8_t*)((int)frame + sizeof(ID3V2FRMHDR));
	}
	
	switch (payloadTextAddress[0]) {
	case ENC_ASCII:
		payloadTextAddress = payloadTextAddress + 1;	//no BOM to take into account
		break;
	case ENC_UTF16LEWBOM:
		payloadTextAddress = payloadTextAddress + 3;	//no BOM to take into account
		break;
	default:
		payloadTextAddress = nullptr;
		break;
	}
	return payloadTextAddress;
}


int calcID3v2SizeField(const uint8_t* number, bool nonStandardCalc) {
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


int GetID3v2PayloadSize(const ID3V2FRMHDR* frame) {
	int payloadSize = 0;
	if (frame != nullptr) {
		payloadSize = calcID3v2SizeField(frame->payloadSize);
		uint8_t* payload = (uint8_t*)frame + 1;
		if (payloadSize > 0) {
			switch (*payload) {
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


bool isID3v2TagIdValid(const ID3V2TAGHDR* tag) {
	bool isValid = false;
	if (tag != nullptr) {
		if ((tag->id[0] == 'I') && (tag->id[1] == 'D') && (tag->id[2] == '3'))
		{
			isValid = true;
		}
	}	
	return isValid;
}


bool isID3v2FrmIdValid(const ID3V2FRMHDR* frame) {
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


ID3V2FRMHDR* getNextID3v2Frame(ID3V2FRMHDR* currentFrame) {

	ID3V2FRMHDR* next = nullptr;
	if (currentFrame != nullptr) {

		int payloadSize = calcID3v2SizeField(currentFrame->payloadSize);

		next = (ID3V2FRMHDR*)((int)currentFrame + 10 + payloadSize);

		if (!isID3v2FrmIdValid(next))
		{
			payloadSize = calcID3v2SizeField(currentFrame->payloadSize, true); //non standard 
			next = (ID3V2FRMHDR*)((int)currentFrame + 10 + payloadSize);
			if (!isID3v2FrmIdValid(next))
			{
				next = nullptr;
			}
		}
	}

	return next;
}


int readID3v2FrmSize(ID3V2FRMHDR* frm)
{
	int size = 0;
	if (frm != nullptr) {
		if (isID3v2FrmIdValid(frm))
		{
			int payloadSize = calcID3v2SizeField(frm->payloadSize);
			size = 10 + payloadSize;
		}
	}

	return size;
}


ID3V2FRMHDR* readID3v2Tag(std::filesystem::path filePath, char* id3v2Tag, int tagsize) {
	ID3V2FRMHDR* firstFrameAddress = nullptr;

	if (isMp3(filePath))
	{
		if (checkID3v2TagExist(filePath))
		{	if (readFile(filePath, id3v2Tag, tagsize))
			{
				firstFrameAddress = (ID3V2FRMHDR*)((int)id3v2Tag + sizeof(ID3V2TAGHDR));
				if (isID3v2FrmIdValid(firstFrameAddress) != true)
				{
					firstFrameAddress = nullptr;
				}
			}//read success			
		}//if there is ID3 Tag
	}//is MP3 file

	return firstFrameAddress;
}


int readID3v2TagSize(ID3V2TAGHDR* hdr)
{
	return calcID3v2SizeField(hdr->size) + 10;
}


void writeID3v2TagSize(ID3V2TAGHDR* hdr, int size)
{
	int temp = size;
	int i = 3;
	while (temp > 0) {

		hdr->size[i] = temp % 128;
		i--;
		temp = temp / 128;
	}
}