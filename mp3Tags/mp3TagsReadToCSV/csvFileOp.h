#pragma once

#include <fstream>
#include "hxlstr.h"
#include "Mp3Tag.h"

void writeColumns(std::ofstream& file);
void writeField(std::ofstream& file, hxlstr& field);
void writeNextLine(std::ofstream& file, Mp3Tag& mp3);