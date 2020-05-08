#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "hxlstr.h"
#include "Mp3Tag.h"
#include "csvFileOp.h"

using namespace std;

//log files
const char nonID3FilesLogFileName[] = "D:\\TUNCA_3_music\\nonID3FilesLog.txt";
const char badID3FilesLogFileName[] = "D:\\TUNCA_3_music\\badID3FilesLog.txt";
const char nonMp3FilesLogFileName[] = "D:\\TUNCA_3_music\\nonMp3FilesLog.txt";

//list of Mp3s
const char output_file_name[] = { "D:\\TUNCA_3_music\\musicArchive.csv" };

const char archivePath[] = { "D:\\TUNCA_3_music\\Music" };
//const char archivePath[] = { "D:\\mp3\\mp3" };

int main()
{	
	ofstream csvFile;
	ofstream nonID3FilesLog;
	ofstream badID3FilesLog;
	ofstream nonMp3FilesLog;	

	csvFile.open(output_file_name);
	nonID3FilesLog.open(nonID3FilesLogFileName, std::ios::binary);
	badID3FilesLog.open(badID3FilesLogFileName, std::ios::binary);
	nonMp3FilesLog.open(nonMp3FilesLogFileName, std::ios::binary);

	if (prepareFiles(csvFile, nonID3FilesLog, badID3FilesLog, nonMp3FilesLog) == true) {		

		for (auto& p : std::filesystem::recursive_directory_iterator(archivePath)) {

			hxlstr input_file_ext = p.path().extension().string().c_str();

			if (isMp3(input_file_ext) == true) {

				Mp3Tag currentTag(p.path());
				writeNextLine(csvFile, currentTag);
				writeLogs(currentTag, nonID3FilesLog, badID3FilesLog, nonMp3FilesLog);

			}//if mp3 file
			
		}//for all files
		closeFiles(csvFile, nonID3FilesLog, badID3FilesLog, nonMp3FilesLog);
	}//if csv file open
	else {
		cout << ".csv file cannot be opened" << endl;
	}
}