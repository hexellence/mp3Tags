
#include "Mp3Tag.h"
#include "csvFileOp.h"
#include "hxlstr.h"

void writeColumns(std::ofstream& file) {
	
	const hxlstr csvColumns[]{ "File Name", SEPARATOR, "Title", SEPARATOR, "Album", SEPARATOR, "Artist", SEPARATOR, "Band", SEPARATOR, "File Path", "\r" };

	if (file.is_open()) {
		file.write(BOM, 2);

		for (auto columnName : csvColumns) {
			file.write(columnName.c_str(), columnName.size());
		}
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