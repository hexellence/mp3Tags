#pragma once

#include <fstream>
#include "hxlstr.h"
#include "Mp3Tag.h"
#include "filesys.h"

const hxlstr SEPARATOR{ "\t" };
const hxlstr CR{ "\r" };
const hxlstr SPACE{ " " };


const hxlstr csvColumns[]{ "FNME", SEPARATOR, "TIT2", SEPARATOR,"TRCK", SEPARATOR, "TALB", SEPARATOR, "TPE1", SEPARATOR, "TPE2", SEPARATOR, "TYER", SEPARATOR,"TCON", SEPARATOR, "COMM", SEPARATOR, "FPTH", "\r" };

void writeColumns(std::ofstream& file);
void writeField(std::ofstream& file, const hxlstr& field);
void writeNextLine(std::ofstream& file, Mp3Tag& mp3);
bool prepareFiles(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog);
void closeFiles(std::ofstream& csvFile, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog);
void writeLogs(Mp3Tag& currentTag, std::ofstream& nonID3FilesLog, std::ofstream& badID3FilesLog, std::ofstream& nonMp3FilesLog);