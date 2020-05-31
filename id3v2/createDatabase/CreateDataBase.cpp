#include <fstream>
#include "csvFileUtil.h"

using namespace std;

//log files
const char nonID3FilesLogFileName[] = "D:\\mp3\\mp3\\nonID3FilesLog.txt";
const char badID3FilesLogFileName[] = "D:\\mp3\\mp3\\badID3FilesLog.txt";
const char nonMp3FilesLogFileName[] = "D:\\mp3\\mp3\\nonMp3FilesLog.txt";

//list of Mp3s
const char output_file_name[] = { "D:\\mp3\\mp3\\musicArchive.txt" };

const char archivePath[] = { "D:\\mp3\\mp3" };


int main()
{
	ofstream csvFile;
	ofstream nonID3FilesLog;
	ofstream badID3FilesLog;
	ofstream nonMp3FilesLog;

	csvFile.open(output_file_name, std::ios::binary);
	nonID3FilesLog.open(nonID3FilesLogFileName, std::ios::binary);
	badID3FilesLog.open(badID3FilesLogFileName, std::ios::binary);
	nonMp3FilesLog.open(nonMp3FilesLogFileName, std::ios::binary);

	if (prepareFiles(csvFile, nonID3FilesLog, badID3FilesLog, nonMp3FilesLog) == true) 
	{
		for (auto& p : std::filesystem::recursive_directory_iterator(archivePath)) 
		{
			if (isMp3(p.path())) 
			{
				//create tag every time a file is opened.
				Id3v2Tag currentTag(p.path());

				if (currentTag.valid())
				{
					//wcout << p.path().generic_wstring() << endl;
					writeNextLine(csvFile, currentTag);					
				}
				else
				{
					hxlstr path = p.path().generic_u16string().c_str();
					writeField(nonID3FilesLog, path);
					writeField(nonID3FilesLog, CR);
				}
			}//if mp3
		}//for all files and folders
		closeFiles(csvFile, nonID3FilesLog, badID3FilesLog, nonMp3FilesLog);
	}//if csv file open
	else 
	{
		cout << ".csv one or more files cannot be opened" << endl;
	}
}