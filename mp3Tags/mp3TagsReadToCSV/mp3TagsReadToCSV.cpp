#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "hxlstr.h"
#include "Mp3Tag.h"
#include "csvFileOp.h"

using namespace std;

//log files
const char nonID3FilesLog[] = "D:\\TUNCA_3_music\\nonID3FilesLog.txt";
const char illegalFlagsLog[] = "D:\\TUNCA_3_music\\illegalFlagsLog.txt";

//list of Mp3s
const char output_file_name[] = { "D:\\TUNCA_3_music\\musicArchive.csv" };

const char archivePath[] = { "D:\\TUNCA_3_music\\Music" };
//const char archivePath[] = { "D:\\mp3\\mp3" };

int main()
{
	hxlstr input_file_ext{};
	ofstream csvFile;
	ofstream nonID3FilesLogFile;
	ofstream illegalFlagsLogFile;

	vector<Mp3Tag> mp3Archive;

	csvFile.open(output_file_name);

	if (csvFile.is_open()) {
		writeColumns(csvFile);

		nonID3FilesLogFile.open(nonID3FilesLog, std::ios::binary);
		illegalFlagsLogFile.open(illegalFlagsLog, std::ios::binary);
		if (illegalFlagsLogFile.is_open() && nonID3FilesLogFile.is_open()) {

			nonID3FilesLogFile.write(BOM, 2);
			illegalFlagsLogFile.write(BOM, 2);			
		}
		else {
			std::cout << "Error openning log files" << std::endl;
		}

		for (auto& p : std::filesystem::recursive_directory_iterator(archivePath)) {

			input_file_ext = p.path().extension().string().c_str();

			if ((input_file_ext == ".mp3") || (input_file_ext == ".MP3") || (input_file_ext == ".Mp3")) {
				
				Mp3Tag currentTag(p.path());
				switch (currentTag.m_status) {
				case 1:
					nonID3FilesLogFile.write(currentTag.m_filePath.c_str(), currentTag.m_filePath.size());
					nonID3FilesLogFile.write(CR.c_str(), CR.size());
					break;
				case 2:
					illegalFlagsLogFile.write(currentTag.m_filePath.c_str(), currentTag.m_filePath.size());
					illegalFlagsLogFile.write(CR.c_str(), CR.size());
					break;
				default:
					break;
				}

				writeNextLine(csvFile, currentTag);

			}//if mp3 file
		}//for all files
		csvFile.close();
		nonID3FilesLogFile.close();
		illegalFlagsLogFile.close();
	}//if csv file open
	else {
		cout << ".csv file cannot be opened" << endl;
	}
}

