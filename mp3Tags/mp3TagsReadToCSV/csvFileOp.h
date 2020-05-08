#pragma once

#include <fstream>
#include "hxlstr.h"
#include "Mp3Tag.h"

const hxlstr SEPARATOR{ "\t" };
const hxlstr CR{ "\r" };
const hxlstr SPACE{ " " };
const char BOM[]{ "\xff\xfe" };


void writeColumns(std::ofstream& file);
void writeField(std::ofstream& file, const hxlstr& field);
void writeNextLine(std::ofstream& file, Mp3Tag& mp3);
bool prepareFiles(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog);
void closeFiles(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog);
bool isMp3(hxlstr input_file_ext);
void writeLogs(Mp3Tag& currentTag, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog);