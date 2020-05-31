
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <filesystem>
#include "hxlstr.h"
#include "Id3v2Tag.h"
#include "csvfileUtil.h"


using namespace std;
using namespace hxl;

bool getColumnNames(std::filesystem::path filePath, vector<hxlstr>& columnNames);
bool readArchiveList(std::filesystem::path filePath, vector<map<hxlstr, hxlstr>>& table);
void setID3Headers(std::filesystem::path filePathIn, std::filesystem::path filePathOut, Id3v2Tag& tag);

std::filesystem::path musicTableFile = { "D:\\mp3\\mp3\\musicArchive.txt" };

int main()
{
	ifstream mp3In;
	ofstream mp3Out;

	vector<map<hxlstr, hxlstr>> archive;

	readArchiveList(musicTableFile, archive);

	for (auto song : archive)
	{
		//for every map item in vector
		std::filesystem::path filePathIn = song["FPTH"].c16_str();
		std::filesystem::path filePathOut = filePathIn.parent_path();
		filePathOut += "/";

		if (filePathIn.has_extension())
		{
			
			bool change = false;
			filePathOut += song["FNME"].c16_str();

			Id3v2Tag tag(filePathIn);

			if (tag.valid())
			{
				for (auto colEntry : song)
				{
					if ((colEntry.first != "FNME") && (colEntry.first != "FPTH"))
					{
						//For every item in map i.e. for every column in a row
						//Id3v2Tag::iterator curFrm = tag.find("TPE1"); //find the matching frame in tag					

						Id3v2Tag::iterator curFrm = tag.find(colEntry.first); //find the matching frame in tag
						if (curFrm != tag.end())
						{
							if (curFrm->value() != colEntry.second)
							{
								//we have an update in this item					
								tag.modify(curFrm, colEntry.second);
								change = true;
							}
						}
						else
						{
							//this does not appear to be in the mp3 file's tag so add new
							if (colEntry.second.size() > 0)
							{
								//checking if we have something to add
								tag.push_back(colEntry.first, colEntry.second);
								change = true;
							}
						}
					}
				}//for each frame in tag
				//save the changes
				if ((change == true) || (filePathOut != filePathOut))
				{
					//save the file with changes
					cout << "Changing: " << filePathIn << endl;
					setID3Headers(filePathIn, filePathOut, tag);
					change = false;
				}//change
			}//if tag is valid
			else
			{
				cout << "Invalid Tag" << endl;

			}
		}

	}// for all songs in the tab separated file
}


bool getColumnNames(std::filesystem::path filePath, vector<hxlstr>& columnNames)
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
		hxlstr newChar = "";
		while (!tabFile.eof())
		{
			tabFile.read((char*)charPair, 2);
			newChar = hxlstr(charPair, 2, hxlstr::ENC::UNICD);
			if (newChar == CR)
			{
				//tabFile.read((char*)charPair, 2); //dummy read
				columnNames.push_back(word);
				isSuccess = true;
				break;
			}
			else if (newChar == SEPARATOR)
			{
				word.trim();
				word.drop(u'\t');
				columnNames.push_back(word);
				word = "";
			}
			else
			{
				word = word + newChar;
			}
		}
	}
	return isSuccess;
}


bool readArchiveList(std::filesystem::path filePath, vector<map<hxlstr, hxlstr>>& table)
{
	bool isSuccess = false;
	vector<hxlstr> columnNames;

	if (getColumnNames(musicTableFile, columnNames))
	{
		ifstream tabFile;
		tabFile.open(filePath, std::ios::binary | std::ios::beg);

		if (tabFile.is_open()) {
			//file is ready
			hxlstr word;	// will be used to hold the content of the columns

			//read BOM
			uint8_t charPair[3]{};
			tabFile.read((char*)charPair, 2);
			hxlstr newChar = "";
			map<hxlstr, hxlstr> row;
			int i = 0;
			while (!tabFile.eof())
			{
				tabFile.read((char*)charPair, 2);
				newChar = hxlstr(charPair, 2, hxlstr::ENC::UNICD);
				if (newChar == CR)
				{
					//tabFile.read((char*)charPair, 2); //dummy read
					row[columnNames[i]] = word;
					table.push_back(row);
					word = "";
					row.clear();
					i = 0;
				}
				else if (newChar == SEPARATOR)
				{
					word.trim();
					word.drop(u'\t');
					row[columnNames[i]] = word;
					word = "";
					i++;
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

int getAudioOffset(std::filesystem::path filePath)
{
	std::ifstream mp3File;
	int fileSize = (int)getFileSize(filePath);
	mp3File.open(filePath, std::ios::binary);
	int offset = getAudioOffset(mp3File, fileSize);
	mp3File.close();
	return offset;
}

void setID3Headers(std::filesystem::path filePathIn, std::filesystem::path filePathOut, Id3v2Tag& tag)
{
	int inFileSize = (int)getFileSize(filePathIn);	

	std::ofstream outFile;
	std::ifstream mp3File;


	int offset = getAudioOffset(filePathIn);


	int audioSize = inFileSize - offset;

	if (audioSize > 0)
	{
		char* audioContent = new char[audioSize];
		mp3File.open(filePathIn, std::ios::binary | std::ios::in);
		if (mp3File.is_open())
		{
			mp3File.seekg(offset, std::ios::beg);
			mp3File.read(audioContent, audioSize);
			mp3File.close();
		}

		outFile.open(filePathOut, std::ios::binary);
		if (outFile.is_open())
		{
			outFile.write(tag.start(), tag.size());	//first write the tag
			outFile.write(audioContent, audioSize);	//place the audio
			//if we need ID3v1.1 add here
			outFile.close();
		}
		delete[] audioContent;
	}
	else {
		std::cout << "file audio detection failed" << std::endl;
	}
}