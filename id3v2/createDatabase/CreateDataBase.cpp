#include <fstream>
#include "csvFileUtil.h"

using namespace std;

//log files
const char nonID3FilesLogFileName[] = "\\Log_NonID3Files.txt";
const char badID3FilesLogFileName[] = "\\Log_badID3Files.txt";
const char nonMp3FilesLogFileName[] = "\\Log_nonMp3Files.txt";

//list of Mp3s
const char outputFileName[] = { "\\musicArchive.txt" };


int main(int argc, char* argv[])
{
	ofstream csvFile;
	ofstream nonID3FilesLog;
	ofstream badID3FilesLog;
	ofstream nonMp3FilesLog;
		
	if (argc == 2)
	{
		std::filesystem::path archivePath = argv[1];		
				
		std::filesystem::path outputFilePath = archivePath;
		std::filesystem::path nonID3FilesLogFilePath = archivePath;
		std::filesystem::path badID3FilesLogFilePath = archivePath;
		std::filesystem::path nonMp3FilesLogFilePath = archivePath;
		
		outputFilePath += outputFileName;
		nonID3FilesLogFilePath += nonID3FilesLogFileName;
		badID3FilesLogFilePath += badID3FilesLogFileName;
		nonMp3FilesLogFilePath += nonMp3FilesLogFileName;
		
		std::filesystem::path listPath = archivePath;
		csvFile.open(outputFilePath, std::ios::binary);
		nonID3FilesLog.open(nonID3FilesLogFilePath, std::ios::binary);
		badID3FilesLog.open(badID3FilesLogFilePath, std::ios::binary);
		nonMp3FilesLog.open(nonMp3FilesLogFilePath, std::ios::binary);		

		if (prepareFiles(csvFile, nonID3FilesLog, badID3FilesLog, nonMp3FilesLog) == true)
		{
			cout << "Creating Database." << endl;
			for (auto& p : std::filesystem::recursive_directory_iterator(archivePath))
			{
				if (isMp3(p.path()))
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
					//create tag every time a file is opened.
					Id3v2Tag currentTag(p.path());

					if (currentTag.valid())
					{
						writeNextLine(csvFile, currentTag);
					}
					else
					{
						//LOG non complying file
						hxlstr path = p.path().generic_u16string().c_str();
						writeField(nonID3FilesLog, path);
						writeField(nonID3FilesLog, CR);
					}
				}//if mp3
				else
				{
					//LOG non complying file
					hxlstr path = p.path().generic_u16string().c_str();
					writeField(nonMp3FilesLog, path);
					writeField(nonMp3FilesLog, CR);
				}
			}//for all files and folders
			closeFiles(csvFile, nonID3FilesLog, badID3FilesLog, nonMp3FilesLog);
		}//if csv file open
		else
		{
			cout << ".csv one or more files cannot be opened" << endl;
		}
	}
}