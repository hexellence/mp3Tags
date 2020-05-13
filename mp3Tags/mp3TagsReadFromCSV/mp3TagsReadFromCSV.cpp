
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

const char input_file_name[] = { "D:\\TUNCA_3_music\\musicArchive.csv" };



void readFileToVector(ifstream& file, vector<vector<hxlstr>>& csv) {

	if (!file) {
		std::cout << "File opening failed\n";
	}
	else {
		//file is ready
		char charPair[3]{};
		file.read(charPair, 2);
		hxlstr newChar = charPair;
		hxlstr word;

		//check BOM
		if (newChar == BOM) {
			vector<hxlstr> column;
			while (!file.eof())
			{
				file.read(charPair, 2);
				newChar = charPair;
				if (newChar == CR)
				{
					file.read(charPair, 2); //dummy read
					column.push_back(word);
					csv.push_back(column);
					word = "";
					column.clear();

				}
				else if (newChar == SEPARATOR)
				{
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
}


int main()
{

	ifstream csvFile;
	ofstream mp3File;
	csvFile.open(input_file_name, std::ios::binary | std::ios::beg);
	vector<vector<hxlstr>> csv;

	readFileToVector(csvFile, csv);

	csvFile.close();

	vector<hxlstr> columnNames = csv[0];

	Mp3Frame frames[8]{};


	char id3TagOut[1000];

	int index = 0;
	for (vector<vector<hxlstr>>::iterator row = csv.begin() + 1; row != csv.end(); ++row) {
		//ignore the first row

		//last column in every row is the path to the file
		std::filesystem::path filePath((char16_t*)((*row)[(*row).size() - 1].c_str()));
		if (filePath.has_filename())
		{
			int columnCount = 1;	//ignore the first column
			for (vector<hxlstr>::iterator col = (*row).begin() + 1; col != (*row).end() - 1; col++)
			{
				//ignore the first and the last columns
				if ((*col).size() > 0) {
					//if there is data in the column create a frame put it in the frames array
					frames[index] = Mp3Frame((*col), columnNames[columnCount]);
					index++;
				}
				columnCount++;
			}
			int tagSize = createID3Tag(id3TagOut, frames, index);

			setID3v2Header(filePath, id3TagOut, tagSize);
			index = 0;
		}
	}
}