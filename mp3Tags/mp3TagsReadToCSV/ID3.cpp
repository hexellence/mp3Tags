#include "ID3.h"

bool getID3Headers(std::filesystem::path filePath, ID3V2HDR* hdrv2, ID3V1HDR* hdrv1) {
	bool retVal = false;
	retVal = getID3v1Header(filePath, hdrv1);
	retVal = retVal && getID3v2Header(filePath, hdrv2);	
	
	return retVal;
}