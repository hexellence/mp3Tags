#pragma once

#include <filesystem>
#include "TagHdr.h"
#include "FileUtil.h"

using namespace std;

class Id3v2Tag
{
private:

	TagHdr* m_pTagHdr;
	uint8_t* m_pWholeTag;
	std::filesystem::path m_path;

	int getID3v2TagHeader(std::filesystem::path filePath, TagHdr* hdr); //getID3v2TagHeader() method reads only the tag hdr and returns the size and returns the Tag header via pointer.
	FrmHdr* readID3v2Tag(std::filesystem::path filePath, uint8_t* id3v2Tag, int tagsize); //readID3v2Tag() method reads the whole tag i.e. tag hdr and frames to a memory area(via pointer), also returns a pointer to the first frame
	static bool checkId(hxlstr id); //checkId() check if this id is a valid Id to instantiate a frame with	

public:
	Id3v2Tag(std::filesystem::path filePath); //Id3v2Tag() Constructor from a file
	~Id3v2Tag(); //~Id3v2Tag() Destructor

	class iterator
	{
	private:
		FrmHdr* m_pos;
		Id3v2Tag& m_theTag;

	public:

		iterator(FrmHdr* pos, Id3v2Tag& aTag); //iterator() constructor to create an iterator
		iterator(const iterator& other); //iterator() copy constructor to create an iterator
		iterator& operator++(int); //operator++(int) postfix increment operator to walk from one frame to the next
		iterator& operator++();
		FrmHdr& operator*();
		FrmHdr* operator->();
		const FrmHdr& operator*() const;
		bool operator!=(const iterator& other) const; //operator!=() this is for comparing the itearators
		const iterator& operator=(const iterator& other); //operator=() this is for assigning
	};

	iterator begin(); //begin() method returns an iterator to the beginning i.e. 10 bytes after the tag header start, regardless there is a valid frame or not
	iterator end(); //end() method returns an iterator that has nothing to do with the actual tag. this is like a terminator
	iterator first(); //first() method is like begin but only returns valid frames.
	iterator last(); //last() returns an iterator to the last valid frame.
	iterator find(hxlstr id); //find() method returns and iterator to the first instance of the matching frame starting from the top.
	bool valid();
	const std::filesystem::path& path();	//returns path of the file
	void del(hxlstr id); //del(hxlstr) deletes the frame with the given id. the later frames will scooch to the front
	void del(iterator it); //del(iterator) deletes the given frame pointed by the iterator
	void push_back(hxlstr id, hxlstr text); //push_back(hxlstr id, hxlstr text) adds the frame with the given parameters to the tag
	void insert(iterator it, hxlstr id, hxlstr text); //insert() inserts a frame pointed by the iterator
	void modify(iterator it, hxlstr text); //insert() inserts a frame pointed by the iterator
	void clear(); //clear() deletes all frames from the tag	
	const char* start();
	int size();
	hxlstr operator[](hxlstr id);	
};

std::ostream& operator<<(std::ostream& out, const Id3v2Tag::iterator& it);