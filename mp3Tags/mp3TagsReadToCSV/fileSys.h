#pragma once

#include <filesystem>

const char BOM[]{ "\xff\xfe" };	//BOM for Unicode 16

bool isMp3(std::filesystem::path filePath);

bool isWma(std::filesystem::path filePath);

void convertEndianness(uint8_t* inout, int size);