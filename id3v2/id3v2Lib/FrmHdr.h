#pragma once

#include <cstdint>
#include "Id3v2Defs.h"
#include "Hdr.h"
#include "hxlstr.h"

using namespace hxl;

class FrmHdr : public Hdr
{
private:
	char _id[4];
	uint8_t _payloadSize[4];
	uint8_t _flags[2];
	uint8_t _payload;

	void size(int size); /*size(int) method lets the class set the payload size when a string is written to a frame.
							This function is not public, as it may cause problems if it is not used carefully.*/
	void flags(); //flags() method clears the flag byte
	void value(hxlstr content); //value(hxlstr content) method writes a string to the payload field.
	static bool valid(const char* id);

public:
	//Constructors
	//don't want automatic consctructors as the memory is assumed to be reserved by the upper level application. 
	//letting constructors can be dangerous as we are walking over a memory area filled from an mp3 file.
	FrmHdr() = delete;
	FrmHdr(const FrmHdr& other) = delete;	
	
	//getters
	bool valid() const; //valid() method checks if the frame is valid.
	hxlstr id() const; //id() method returns the id of the frame.
	int size() const; //size() method returns the size of the payload.
	int bytes() const; //bytes() method returns the size of the payload and the header.
	FrmHdr* next() const; //next() method returns a pointer to the next frame.
	hxlstr value() const; //value() method returns a copy of the string in the frame.
	const FrmHdr* start() const; //start() method returns the address of the header.
	
	//setters
	void id(hxlstr id); //id(hxlstr) method writes a new Id to a frame
	friend class Id3v2Tag;
};

std::ostream& operator<<(std::ostream& out, const FrmHdr& frm);
