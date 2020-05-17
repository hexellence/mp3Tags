
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "hxlstr.h"
#include "csvFileOp.h"
#include "ID3.h"
#include "mp3Frame.h"

using namespace std;
using namespace hxl;

bool readFileToVector(std::filesystem::path filePath, vector<vector<hxlstr>>& table);

std::filesystem::path musicTableFile = { "D:\\mp3\\mp3\\musicArchive.txt" };



int main()
{
	ID3V2HDR id3v2Hdr;
	ID3V1HDR id3v1Tag;
	ofstream mp3File;

	vector<vector<hxlstr>> musicTable;
	readFileToVector(musicTableFile, musicTable);

	vector<hxlstr> columnNames = musicTable[0];	//0th row is the column names i.e. the field IDs

	if (musicTable.size() > 1)	//if there is data in it there must be more than 1 row
	{
		for (vector<vector<hxlstr>>::iterator row = musicTable.begin() + 1; row != musicTable.end(); ++row)
		{
			//ignore the first row
			//last column in every row is the path to the file
			std::filesystem::path filePath((char16_t*)((*row)[(*row).size() - 1].c16_str()));

			if (filePath.has_filename()) {
				int notExistingFrameCount = 0;
				Mp3Frame* notExistingFrames = new Mp3Frame[(*row).size()];
				
				getID3v1Header(filePath, &id3v1Tag);

				if (checkifID3V2TagExist(filePath))
				{
					int fileSize = (int)getFileSize(filePath);

					getID3v2Header(filePath, &id3v2Hdr);					
					int tagSize = getTagSize(&id3v2Hdr);
					//we will increase the size thus lets reserve more than we need
					char* tagDataIn = new char[tagSize * 2];
					char* tagDataInAlt = new char[tagSize * 2];
					memset(tagDataInAlt, 0, tagSize * 2);
					getTag(filePath, tagDataIn);
					int newTagsize = 0;

					for (int c = 1; c < (int)(*row).size() - 1; ++c)
					{
						//for all column
						bool frameHandled = false;
						//for all columns in the row
						if ((*row)[c].size() > 0)
						{
							char* lastSectionAddress = nullptr;
							int lastSectionSize = 0;
							char* tagLastAddress = nullptr;
							int firstSectionSize = 0;

							ID3V2FRM* currentFrame = (ID3V2FRM*)&tagDataIn[10];
							//if the field is populated
							while (currentFrame != nullptr)
							{
								frameHandled = false;
								if (hxlstr((uint8_t*)currentFrame->id, 4, hxlstr::ENC::ASCII) == columnNames[c])
								{
									char* tagLastAddress = &tagDataIn[tagSize - 1];
									int firstSectionSize = (int)currentFrame - (int)tagDataIn;

									memcpy(tagDataInAlt, tagDataIn, firstSectionSize);
									newTagsize = firstSectionSize;

									Mp3Frame frame(((*row)[c]), columnNames[c]);
									int newFrmSize = createID3v2Frame(frame, &tagDataInAlt[firstSectionSize]);
									newTagsize += newFrmSize;

									lastSectionAddress = (char*)((int)currentFrame + getFrmSize(currentFrame));
									lastSectionSize = (int)tagLastAddress - (int)lastSectionAddress;

									memcpy(&tagDataInAlt[firstSectionSize + newFrmSize], lastSectionAddress, lastSectionSize);
									newTagsize += lastSectionSize;

									//swap the regions for efficiency
									char* temp = tagDataIn;	//store the pointer
									tagDataIn = tagDataInAlt;
									tagDataInAlt = temp;
									memset(tagDataInAlt, 0, newTagsize);
									//update tag size
									tagSize = newTagsize;
									frameHandled = true;
									break;
								}
								currentFrame = getNextFrame(currentFrame);
							}//while already present frames						
							if (!frameHandled)
							{
								notExistingFrames[notExistingFrameCount] = Mp3Frame(((*row)[c]), columnNames[c]);
								frameHandled = true;
							}
						}// column populated				
					}//for all columns

					 //We could not find some frames Ex: TALB and TIT2 in the file				
					for (int i = 0; i < notExistingFrameCount; ++i)
					{
						//create the frame at the end of the tag												
						int newFrmSize = createID3v2Frame(notExistingFrames[i], &tagDataIn[tagSize - 1]);
						tagSize += newFrmSize;
					}//for all not existing items
					delete[] notExistingFrames;
					createID3v1Tag(&id3v1Tag, notExistingFrames, notExistingFrameCount);
					setID3Headers(filePath, (char16_t*)((*row)[0].c16_str()), tagDataIn, &id3v1Tag, tagSize);

					delete[] tagDataIn;
					delete[] tagDataInAlt;
				} //modify already present ID3v2 Tag
				else
				{
					//create a whole new ID3v2 Tag because the track doesnt have one
					char* tagDataIn = new char[1000];
					Mp3Frame frames[10]{};
					int numOfFrames = 0;

					for (int c = 1; c < (int)(*row).size() - 1; ++c)
					{
						//for all columns in the row
						if ((*row)[c].size() > 0)
						{
							//check if column has data
							frames[numOfFrames] = Mp3Frame(((*row)[c]), columnNames[c]);
							numOfFrames++;
						}
					}

					int id3v2tagSize = createID3v2Tag(tagDataIn, frames, numOfFrames);//ignore the first frame as it is not an actual frame
					createID3v1Tag(&id3v1Tag, frames, numOfFrames);
					setID3Headers(filePath, (char16_t*)((*row)[0].c_str()), tagDataIn, &id3v1Tag, id3v2tagSize);
					delete[] tagDataIn;
				}//add a new ID3v2 tag as it does not exist
			}//if there is a file
		}//for all files listed

	}//if there is any file in the list


	//int tagSize = createID3Tag(id3TagOut, &frames[1], index - 1);//ignore the first frame as it is not an actual frame

	//setID3v2Header(filePath, (char16_t*)frames[0].m_str.c_str(), id3TagOut, tagSize);
	//if at least a file is in there
}


bool readFileToVector(std::filesystem::path filePath, vector<vector<hxlstr>>& table)
{
	bool isSuccess = false;

	ifstream tabFile;
	tabFile.open(filePath, std::ios::binary | std::ios::beg);

	if (tabFile.is_open()) {
		//file is ready
		hxlstr word;	// will be used to hold the content of the columns

		//read BOM
		uint8_t charPair[3]{};
		tabFile.read((char*)charPair, 2);
		hxlstr newChar(charPair, 2, hxlstr::ENC::ASCII);

		//check BOM
		if (newChar == BOM) {
			isSuccess = true;
			vector<hxlstr> column;
			while (!tabFile.eof())
			{
				tabFile.read((char*)charPair, 2);
				newChar = hxlstr(charPair, 2, hxlstr::ENC::UTF16LE);
				if (newChar == CR)
				{
					//tabFile.read((char*)charPair, 2); //dummy read
					column.push_back(word);
					table.push_back(column);
					word = "";
					column.clear();
				}
				else if (newChar == SEPARATOR)
				{
					word.trim();
					word.remove(u'\t');
					column.push_back(word);
					word = "";
				}
				else
				{
					word = word + newChar;
				}
			}//do until the eof
		} //if there is BOM
	} // file ready    
	return isSuccess;
}