#include "TagHdr.h"

/*
	valid() method returns true if the tag is starting with the expected characters
*/
bool TagHdr::valid() const
{
	return (id() == "ID3");
}

/*
	id() method returns the id of the tag
*/
hxlstr TagHdr::id() const
{
	return hxlstr(_id, 3);
}

/*
	size() method returns the size of the tag
*/
int TagHdr::size() const
{
	return calcID3v2SizeField(_size);
}

/*
	bytes() method returns the whole size of the tag including the header
*/
int TagHdr::bytes() const
{
	return size() + hdr_size();
}

/*
	first() returns the first frame address regardless there is a valid frame
*/
FrmHdr* TagHdr::first()
{
	FrmHdr* firstFrm = (FrmHdr*)&_firstFrm;
	return firstFrm;
}

/*
	size(int) sets the size of the tag
*/
void TagHdr::size(int size)
{
	int temp = size;
	int i = 3;
	while (temp > 0) {

		_size[i] = temp % 128;
		i--;
		temp = temp / 128;
	}
}