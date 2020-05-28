
#include "Id3v2Defs.h"

bool checkId(hxl::hxlstr id)
{
	bool idFound = false;
	for (auto item : ID3_FrameDefs) {
		if (id == item.sLongTextID)
		{
			idFound = true;
			break;
		}
	}
	return idFound;
}