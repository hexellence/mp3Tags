#pragma once
#include <filesystem>

const char BOMLE[]{ "\xff\xfe" };	//BOM for Unicode 16

bool isMp3(std::filesystem::path filePath);
bool isWma(std::filesystem::path filePath);
int getFileSize(std::filesystem::path filename);
bool readFile(std::filesystem::path filePath, char* data, int sizeDirectionToRead);
bool readFile(std::filesystem::path filePath, char* data, int startOffset, int endOffset);