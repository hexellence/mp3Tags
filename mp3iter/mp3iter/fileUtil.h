#pragma once
#include <filesystem>

const char BOM[]{ "\xff\xfe" };	//BOM for Unicode 16

bool isMp3(std::filesystem::path filePath);

bool isWma(std::filesystem::path filePath);

void convertEndianness(uint8_t* inout, int size);

int getFileSize(std::filesystem::path filename);

bool readFile(std::filesystem::path filePath, char* data, int sizeDirectionToRead);
bool readFile(std::filesystem::path filePath, char* data, int startOffset, int endOffset);