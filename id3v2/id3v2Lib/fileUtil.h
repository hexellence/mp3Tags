#pragma once
#include <filesystem>

const uint8_t mp3AudioHeader[2]{ 0xFF, 0xFB };

bool isMp3(std::filesystem::path filePath);
bool isWma(std::filesystem::path filePath);
int getFileSize(std::filesystem::path filename);
bool readFile(std::filesystem::path filePath, char* data, int sizeDirectionToRead);
bool readFile(std::filesystem::path filePath, char* data, int startOffset, int endOffset);

int getAudioOffset(std::ifstream& mp3File, int size);
int getAudioOffset(std::filesystem::path filePath);
int getID3v2TagSize(std::filesystem::path filePath);