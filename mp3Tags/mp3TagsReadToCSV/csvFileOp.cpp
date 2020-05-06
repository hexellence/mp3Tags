
#include "csvFileOp.h"


void writeColumns(std::ofstream& file) {
	hxlstr separator{ "\t" };
	hxlstr csvColumns[]{ "File Name", separator, "Title", separator, "Album", separator, "Artist", separator, "Band", separator, "File Path", "\r" };

	if (file.is_open()) {
		file.write("\xff\xfe", 2);	//BOM

		for (auto columnName : csvColumns) {
			file.write(columnName.c_str(), columnName.size());
		}
	}
}

