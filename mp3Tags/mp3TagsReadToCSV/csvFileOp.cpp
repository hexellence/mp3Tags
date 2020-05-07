
#include "Mp3Tag.h"
#include "csvFileOp.h"
#include "hxlstr.h"

hxlstr separator{ "\t" };
hxlstr cr{ "\r" };

void writeColumns(std::ofstream& file) {
	
	hxlstr csvColumns[]{ "File Name", separator, "Title", separator, "Album", separator, "Artist", separator, "Band", separator, "File Path", cr };

	if (file.is_open()) {
		file.write("\xff\xfe", 2);	//BOM

		for (auto columnName : csvColumns) {
			file.write(columnName.c_str(), columnName.size());
		}
	}
}


void writeField(std::ofstream& file, hxlstr& field) {
	if (field.c_str() != nullptr) {
		file.write(field.c_str(), field.size());
	}
	else {
		file.write((char*)" ", sizeof(1));
	}
}


void writeNextLine(std::ofstream& file, Mp3Tag& mp3) {

	writeField(file, mp3.m_fileName);	
	writeField(file, separator);
	writeField(file, mp3.m_title);
	writeField(file, separator);
	writeField(file, mp3.m_album);
	writeField(file, separator);
	writeField(file, mp3.m_artist1);
	writeField(file, separator);
	writeField(file, mp3.m_artist2);
	writeField(file, separator);
	writeField(file, mp3.m_filePath);
	writeField(file, cr);	
}