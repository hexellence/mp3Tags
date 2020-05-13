#pragma once

#include <memory>
#include <iostream>
#include "hxlstr.h"
#include "ID3V2Tag.h"
#include "ID3V1Tag.h"

using namespace hxl;

class Mp3Frame {

private:

public:
	hxlstr m_str;
	hxlstr m_id;

	//default constructor
	Mp3Frame();

	//constructor from buffer
	Mp3Frame(ID3V2FRM* frame);	

	//constructor from normal char array
	Mp3Frame(const char* init, const char* id);

	//copy constructor
	Mp3Frame(const Mp3Frame& other) = delete;

	//destructor
	~Mp3Frame();

	//assignment operator
	const Mp3Frame& operator=(const Mp3Frame& other);

	//assignment operator from char16_t
	const Mp3Frame& operator=(const char16_t* other);

	const hxlstr& id();

	const hxlstr& text();

};


int createID3v2Frame(Mp3Frame& frameIn, char* frameOut);

