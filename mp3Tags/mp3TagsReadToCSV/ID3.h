#pragma once
#include <filesystem>
#include "ID3V2Tag.h"
#include "ID3V1Tag.h"



bool getID3Headers(std::filesystem::path filePath, ID3V2HDR* hdrv2, ID3V1HDR* hdrv1);