
#include "Mp3Tag.h"
#include "csvFileOp.h"
#include "hxlstr.h"

bool prepareFiles(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog) {
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



void writeColumns(std::ofstream& file) {
	
	const hxlstr csvColumns[]{ "File Name", SEPARATOR, "Title", SEPARATOR, "Album", SEPARATOR, "Artist", SEPARATOR, "Band", SEPARATOR, "File Path", "\r" };
	file.write(BOM, 2);
	for (auto columnName : csvColumns) {
		file.write(columnName.c_str(), columnName.size());
	}
	
}


void writeField(std::ofstream& file, const hxlstr& field) {
	if (field.c_str() != nullptr) {
		file.write(field.c_str(), field.size());
	}
	else {
		file.write(SPACE.c_str(), SPACE.size());
	}
}


void writeNextLine(std::ofstream& file, Mp3Tag& mp3) {

	writeField(file, mp3.m_fileName);	
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_title);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_album);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_artist1);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_artist2);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_filePath);
	writeField(file, CR);
}


void writeLogs(Mp3Tag& currentTag, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog) {

	switch (currentTag.m_status) {
	case 1:
		nonID3FilesLog.write(currentTag.m_filePath.c_str(), currentTag.m_filePath.size());
		nonID3FilesLog.write(CR.c_str(), CR.size());
		break;
	case 2:
		badID3FilesLog.write(currentTag.m_filePath.c_str(), currentTag.m_filePath.size());
		badID3FilesLog.write(CR.c_str(), CR.size());
		break;
	default:
		break;
	}
}

void closeFiles(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog) {
	csvFile.close();
	nonID3FilesLog.close();
	badID3FilesLog.close();
	nonMp3FilesLog.close();
}

bool filesOpen(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog) {
	bool retVal = csvFile.is_open() && nonID3FilesLog.is_open() && badID3FilesLog.is_open() && nonMp3FilesLog.is_open();
	return retVal;
}


bool isMp3(hxlstr input_file_ext) {
	bool retVal = (input_file_ext == ".mp3") || (input_file_ext == ".MP3") || (input_file_ext == ".Mp3");
	return retVal;
}