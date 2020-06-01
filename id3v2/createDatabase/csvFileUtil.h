#pragma once
#include <fstream>
#include "hxlstr.h"
#include "id3v2Tag.h"


using namespace std;
using namespace hxl;

const hxlstr SEPARATOR{ "\t" };
const hxlstr CR{ "\r" };
const hxlstr SPACE{ " " };
const hxlstr csvColumns[]{ "FNME", SEPARATOR, "TIT2", SEPARATOR,"TRCK", SEPARATOR, "TALB", SEPARATOR, "TPE1", SEPARATOR, "TPE2", SEPARATOR, "TYER", SEPARATOR,"TCON", SEPARATOR, "FPTH", "\r" };
const char BOM[]{ "\xff\xfe" };	//BOM for Unicode 16 Little Endian

void writeColumns(std::ofstream& file);
void writeField(std::ofstream& file, const hxlstr& field);
void writeNextLine(std::ofstream& file, Id3v2Tag& mp3);
bool prepareFiles(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog);
void closeFiles(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog);
void writeLogs(Id3v2Tag& currentTag, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog);
