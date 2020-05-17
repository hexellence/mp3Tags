
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
	
	file.write(BOM, 2);
	for (auto columnName : csvColumns) {
		file.write(columnName.raw(), columnName.size());
	}
	
}


void writeField(std::ofstream& file, const hxlstr& field) {

	if (field.c_str() != nullptr) {
		file.write(field.raw(), field.size());
	}
	else {
		file.write(SPACE.raw(), SPACE.size());
	}
}


void writeNextLine(std::ofstream& file, Mp3Tag& mp3) {

	writeField(file, mp3.m_fileName);	
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_title.m_str);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_trackNo.m_str);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_album.m_str);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_artist1.m_str);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_artist2.m_str);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_year.m_str);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_genre.m_str);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_comment.m_str);
	writeField(file, SEPARATOR);
	writeField(file, mp3.m_filePath);
	writeField(file, CR);
}


void writeLogs(Mp3Tag& currentTag, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog) {

	switch (currentTag.m_status) {
	case Mp3Tag::STATUS::MP3TAG_NO_ID3V1:
		writeField(nonID3FilesLog, currentTag.m_filePath);
		writeField(nonID3FilesLog, CR);
		break;
	case Mp3Tag::STATUS::MP3TAG_ILLFORMED_ID3V1:
	case Mp3Tag::STATUS::MP3TAG_ILLFORMED_ID3V2:
		writeField(badID3FilesLog, currentTag.m_filePath);
		writeField(badID3FilesLog, CR);
		break;
	case Mp3Tag::STATUS::MP3TAG_NOT_MP3:
		writeField(nonMp3FilesLog, currentTag.m_filePath);
		writeField(nonMp3FilesLog, CR);
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

