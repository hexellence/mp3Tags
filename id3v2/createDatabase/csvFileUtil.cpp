#include "csvFileUtil.h"

using namespace hxl;

bool prepareFiles(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog) 
{
	bool retVal = true;
	if (csvFile.is_open() && nonID3FilesLog.is_open() && badID3FilesLog.is_open() && nonMp3FilesLog.is_open()) {

		writeColumns(csvFile);
		nonID3FilesLog.write(BOM, 2);
		badID3FilesLog.write(BOM, 2);
		nonMp3FilesLog.write(BOM, 2);
	}
	else {
		retVal = false;
		std::cout << "Error openning log files" << std::endl;
	}
	return retVal;
}


void writeColumns(std::ofstream& file) 
{

	file.write(BOM, 2);
	for (auto columnName : csvColumns) {		
		file.write((const char*)columnName.raw(), (long long)columnName.size());
	}
}


void writeField(std::ofstream& file, const hxlstr& field) 
{
	file.write((const char*)field.raw(), (long long)field.size());
}


void writeNextLine(std::ofstream& file, Id3v2Tag& mp3) 
{

	hxlstr fname = hxlstr(mp3.path().filename().generic_u16string().c_str());
	writeField(file, fname);
	writeField(file, SEPARATOR);	
	
	for (int i = 2; csvColumns[i] != "FPTH"; i = i + 2)
	{
		//std::cout << csvColumns[i] << std::endl;
		writeField(file, mp3[csvColumns[i]]);
		writeField(file, SEPARATOR);
	}		
		
	writeField(file, mp3.path().generic_u16string().c_str());
	writeField(file, CR);
}


void writeLogs(Id3v2Tag& currentTag, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog) 
{


}

void closeFiles(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog) 
{
	csvFile.close();
	nonID3FilesLog.close();
	badID3FilesLog.close();
	nonMp3FilesLog.close();
}

bool filesOpen(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog) 
{
	bool retVal = csvFile.is_open() && nonID3FilesLog.is_open() && badID3FilesLog.is_open() && nonMp3FilesLog.is_open();
	return retVal;
}