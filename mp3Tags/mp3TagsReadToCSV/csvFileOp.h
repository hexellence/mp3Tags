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