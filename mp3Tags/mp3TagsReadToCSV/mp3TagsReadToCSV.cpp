#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "hxlstr.h"
#include "Mp3Tag.h"
#include "csvFileOp.h"

using namespace std;

string output_file_name{ "D:\\TUNCA_3_music\\musicArchive.csv" };

//const char archivePath[] = { "D:\\TUNCA_3_music\\Music" };
const char archivePath[] = { "D:\\mp3\\mp3" };

int main()
{
	hxlstr input_file_ext{};
	ofstream csvFile;

	vector<Mp3Tag> mp3Archive;

	csvFile.open(output_file_name);

	if (csvFile.is_open()) {

		writeColumns(csvFile);

		for (auto& p : std::filesystem::recursive_directory_iterator(archivePath)) {

			input_file_ext = p.path().extension().string().c_str();

			if ((input_file_ext == ".mp3") || (input_file_ext == ".MP3") || (input_file_ext == ".Mp3")) {
				
				Mp3Tag currentTag(p.path());
				writeNextLine(csvFile, currentTag);

			}//if mp3 file
		}//for all files
	}//if csv file open

}

