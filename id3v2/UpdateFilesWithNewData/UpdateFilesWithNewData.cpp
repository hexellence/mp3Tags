
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <filesystem>
#include "hxlstr.h"
#include "Id3v2Tag.h"
#include "csvfileUtil.h"
#include "fileUtil.h"


using namespace std;
using namespace hxl;

bool getColumnNames(std::filesystem::path filePath, vector<hxlstr>& columnNames);
bool readArchiveList(std::filesystem::path filePath, vector<map<hxlstr, hxlstr>>& table);
void setID3Headers(std::filesystem::path filePathIn, std::filesystem::path filePathOut, Id3v2Tag& tag);


int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		ifstream mp3In;
		ofstream mp3Out;

		vector<map<hxlstr, hxlstr>> archive;

		std::filesystem::path musicTableFile = argv[1];

		if (readArchiveList(musicTableFile, archive))
		{
			cout << endl << "Started updating mp3 files" << flush;
			for (auto song : archive)
			{
				{
					static int count = 0;
					count++;
					if (count == 100)
					{
						cout << "." << flush;
						count = 0;
					}
				}

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
						if ((change == true) || (filePathIn != filePathOut))
						{
							//save the file with changes
							cout << "." << flush;
							if (filePathIn == filePathOut)
							{
								auto newPath = filePathOut.parent_path();
								auto newName = filePathOut.filename();
								newName += "_hxl.mp3";
								newPath += "/";
								newPath += newName;
								filePathOut = newPath;
							}											

							setID3Headers(filePathIn, filePathOut, tag);
							change = false;
						}//change
					}//if tag is valid
					else
					{
						cout << "Invalid Tag" << endl;
					}
				}//has extension
			}// for all songs in the tab separated file
		}//read archieve
		else
		{
			cout << "archive list cannot be read" << endl;
		}
	}
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
		if ((charPair[0] == BOM[0]) && (charPair[1] == BOM[1]))
		{
			while (!tabFile.eof())
			{
				tabFile.read((char*)charPair, 2);
				newChar = hxlstr(charPair, 2, hxlstr::ENC::UNICD);
				if (newChar == CR)
				{
					//tabFile.read((char*)charPair, 2); //dummy read
					word.trim(hxlstr('"'));
					columnNames.push_back(word);
					isSuccess = true;
					break;
				}
				else if (newChar == SEPARATOR)
				{
					word.trim();
					word.trim(hxlstr('"'));
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
		else
		{
			cout << "BOM not found" << endl;
		}
	}
	return isSuccess;
}


bool readArchiveList(std::filesystem::path filePath, vector<map<hxlstr, hxlstr>>& table)
{
	bool isSuccess = false;
	vector<hxlstr> columnNames;

	if (getColumnNames(filePath, columnNames))
	{

		ifstream tabFile;
		tabFile.open(filePath, std::ios::binary | std::ios::beg);		
		if (tabFile.is_open()) {
			//file is ready
			hxlstr word;	// will be used to hold the content of the columns

			cout << "Reading database" << flush;

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
					{
						static int count = 0;
						count++;
						if (count == 100)
						{
							cout << "." << flush;
							count = 0;
						}
					}

					//tabFile.read((char*)charPair, 2); //dummy read
					if((columnNames[i] == "FNME") || (columnNames[i] == "FPTH"))
						word.trim(hxlstr('"'));
					row[columnNames[i]] = word;
					table.push_back(row);
					word = "";
					row.clear();
					i = 0;
					isSuccess = true;
				}
				else if (newChar == SEPARATOR)
				{
					word.trim();
					if ((columnNames[i] == "FNME") || (columnNames[i] == "FPTH"))
						word.trim(hxlstr('"'));
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
		else 
		{
			std::cout << "file cannot be opened for writing" << std::endl;
		}
		delete[] audioContent;
	}
	else {
		std::cout << "file audio detection failed" << std::endl;
	}
}