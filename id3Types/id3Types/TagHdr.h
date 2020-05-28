#pragma once
#include "Hdr.h"
#include "FrmHdr.h"

class TagHdr : public Hdr
{
private:
	char _id[3];
	uint8_t _ver[2];
	uint8_t _flags;
	uint8_t _size[4];
	char _firstFrm;

public:
	//Don't want any auto constructors as the memory will be provided 
	//by reserving a memory and all functions work on that memory area
	TagHdr() = delete;
	TagHdr(const TagHdr& other) = delete;

	//getters
	bool valid() const; //valid() method returns true if the tag is starting with the expected characters	
	hxlstr id() const; //id() method returns the id of the tag	
	int size() const;//size() method returns the size of the tag
	int bytes() const; //bytes() method returns the whole size of the tag including the header
	
	//setters
	FrmHdr* first(); //first() returns the first frame address regardless there is a valid frame		
	void size(int size); //size(int) sets the size of the tag
};