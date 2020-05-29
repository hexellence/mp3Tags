#include "Id3v2Tag.h"

/*
	getID3v2TagHeader() method reads only the tag hdr and returns the size and returns the Tag header via pointer.
*/
int Id3v2Tag::getID3v2TagHeader(std::filesystem::path filePath, TagHdr* hdr)
{
	int tagSize = 0;
	if (readFile(filePath, (char*)hdr, sizeof(*hdr)))
	{
		if (hdr->valid())
		{
			tagSize = hdr->size();
		}
	}
	return tagSize;
}

/*
	readID3v2Tag() method reads the whole tag i.e. tag hdr and frames to a memory area(via pointer), also returns a pointer to the first frame
*/
FrmHdr* Id3v2Tag::readID3v2Tag(std::filesystem::path filePath, uint8_t* id3v2Tag, int tagsize) {
	FrmHdr* firstFrame = nullptr;

	if (readFile(filePath, (char*)id3v2Tag, tagsize))
	{
		firstFrame = (FrmHdr*)((int)id3v2Tag + Hdr::hdr_size());
		if (firstFrame->valid() != true)
		{
			firstFrame = nullptr;
		}
	}//read success	
	return firstFrame;
}

/*
	checkId() check if this id is a valid Id to instantiate a frame with
*/
bool Id3v2Tag::checkId(hxlstr id)
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


/*
	Id3v2Tag() Constructor from a file
*/
Id3v2Tag::Id3v2Tag(std::filesystem::path filePath) {

	//getting the size first
	char* pTagHdr = new char[sizeof(TagHdr)];
	int size = getID3v2TagHeader(filePath, (TagHdr*)pTagHdr);
	delete[] pTagHdr;

	// if size is successful
	if (size > 0)
	{
		
		m_pWholeTag = new uint8_t[size * 2];
		FrmHdr* firstFrame = readID3v2Tag(filePath, m_pWholeTag, size + Hdr::hdr_size());
		if (firstFrame != nullptr)
		{
			m_pTagHdr = (TagHdr*)m_pWholeTag;
			m_path = filePath;
		}
	}
}

/*
	~Id3v2Tag() Destructor
*/
Id3v2Tag::~Id3v2Tag()
{
	delete[] m_pWholeTag;
}

/*
	iterator() constructor to create an iterator
*/
Id3v2Tag::iterator::iterator(FrmHdr* pos, Id3v2Tag& aTag) : m_pos(pos), m_theTag(aTag) {}

/*
	iterator() copy constructor to create an iterator
*/
Id3v2Tag::iterator::iterator(const iterator& other) : m_pos(other.m_pos), m_theTag(other.m_theTag) {}

/*
	operator++(int) postfix increment operator to walk from one frame to the next
*/
Id3v2Tag::iterator& Id3v2Tag::iterator::operator++(int)
{
	m_pos = m_pos->next();
	return *this;
}

/*
	operator++() prefix increment operator to walk from one frame to the next
*/
Id3v2Tag::iterator& Id3v2Tag::iterator::operator++()
{
	m_pos = m_pos->next();
	return *this;
}

/*
	operator*() this is for returning the base type of the tag i.e. frame
*/
FrmHdr& Id3v2Tag::iterator::operator*()
{
	return *m_pos;
}

const FrmHdr& Id3v2Tag::iterator::operator*() const
{
	return *m_pos;
}

/*
	operator!=() this is for comparing the itearators
*/
bool Id3v2Tag::iterator::operator!=(const iterator& other) const {
	return m_pos != other.m_pos;
}

/*
	operator=() this is for assigning
*/
const Id3v2Tag::iterator& Id3v2Tag::iterator::operator=(const iterator& other) {
	m_pos = other.m_pos;
	m_theTag = other.m_theTag;
	return *this;
}


/*
	begin() method returns an iterator to the beginning i.e. 10 bytes after the tag header start, regardless there is a valid frame or not
*/
Id3v2Tag::iterator Id3v2Tag::begin()
{
	return iterator(m_pTagHdr->first(), *this);
}

/*
	end() method returns an iterator that has nothing to do with the actual tag. this is like a terminator
*/
Id3v2Tag::iterator Id3v2Tag::end()
{
	return iterator(nullptr, *this);
}

/*
	first() method is like begin but only returns valid frames.
*/
Id3v2Tag::iterator Id3v2Tag::first()
{
	iterator it = end();
	if (m_pTagHdr->first()->valid())
	{
		it = iterator(m_pTagHdr->first(), *this);
	}
	return it;
}

/*
	last() returns an iterator to the last valid frame.
*/
Id3v2Tag::iterator Id3v2Tag::last()
{
	iterator it = begin();
	iterator itLast = begin();
	while (it != end())
	{
		itLast = it;
		it++;
	}
	return itLast;
}

/*
	find() method returns and iterator to the first instance of the matching frame starting from the top.
*/
Id3v2Tag::iterator Id3v2Tag::find(hxlstr id) 
{
	iterator it = iterator(m_pTagHdr->first(), *this);
	while (it != end())
	{
		if ((*it).id() == id)
		{
			break;
		}
		it++;
	}
	return it;
}

/*
	del(hxlstr) deletes the frame with the given id. the later frames will scooch to the front
*/
void Id3v2Tag::del(hxlstr id)
{
	iterator it = find(id);
	del(it);
}

/*
	del(iterator) deletes the given frame pointed by the iterator
*/
void Id3v2Tag::del(iterator it)
{
	if (it != end())
	{
		FrmHdr* frmToDelete = &(*it);
		//FrmHdr* frmToDelete = find(id);
		int frameSizeDelete = frmToDelete->bytes();

		//overwrite old frame
		FrmHdr* nextFrame = frmToDelete->next();
		if (nextFrame == nullptr)
		{
			//if it was the last frame hypotetical nextFrame would be at this address
			nextFrame = (FrmHdr*)((int)frmToDelete + frameSizeDelete);
		}

		int lastAddress = (int)m_pTagHdr + m_pTagHdr->bytes();
		int moveSize = lastAddress - (int)nextFrame;
		memcpy(frmToDelete, nextFrame, moveSize);

		//update Tag size		
		m_pTagHdr->size(m_pTagHdr->size() - frameSizeDelete);
	}
}

/*
	push_back(hxlstr id, hxlstr text) adds the frame with the given parameters to the tag
*/
void Id3v2Tag::push_back(hxlstr id, hxlstr text)
{
	if (checkId(id))
	{
		iterator it = last();
		FrmHdr* lastFrm = &(*it);

		int lastFrameSize = 0;
		if (lastFrm->valid())
		{
			lastFrameSize = lastFrm->bytes();
		}

		FrmHdr* newFrm = (FrmHdr*)((int)lastFrm + lastFrameSize);

		newFrm->id(id);
		newFrm->flags();
		newFrm->value(text);

		//update Tag size
		m_pTagHdr->size(m_pTagHdr->size() + newFrm->bytes());
	}
}

/*
	insert() inserts a frame pointed by the iterator
*/
void Id3v2Tag::insert(iterator it, hxlstr id, hxlstr text)
{
	if ((it != end()) && checkId(id))
	{
		int newFrmSize = (text.enc() == hxlstr::ENC::ASCII) ? text.size() + 1 : text.size() + 3;
		newFrmSize += Hdr::hdr_size();

		FrmHdr* presentFrm = &(*it);	//this frame will slide as much as our new frame size is	
		FrmHdr* newLocation = (FrmHdr*)((int)presentFrm + newFrmSize); //present frame's new location

		//calculate how much data will be moved
		int lastAddress = (int)m_pTagHdr + m_pTagHdr->bytes();
		int moveSize = lastAddress - (int)presentFrm;

		//copy and make space where the former frame was
		if (moveSize > 0)
		{
			memcpy(newLocation, presentFrm, moveSize);
		}
		else
		{
			cout << "sizeProblem" << endl;
		}

		//presentFrm will now contain new
		presentFrm->id(id);
		presentFrm->flags();
		presentFrm->value(text);
		//update Tag size		
		m_pTagHdr->size(m_pTagHdr->size() + newFrmSize);
	}
}

/*
	clear() deletes all frames from the tag
*/
void Id3v2Tag::clear()
{
	iterator it = begin();

	while ((*it).valid())
	{
		del(it);
	}
}

/*
	path() returns the path to the file
*/
const std::filesystem::path& Id3v2Tag::path()
{
	return m_path;
}


std::ostream& operator<<(std::ostream& out, const Id3v2Tag::iterator& it) {
	out << (*it).id() << ": " << (*it).value() << std::flush;
	return out;
}


/*
	operator[] works only to read
*/
hxlstr Id3v2Tag::operator[](hxlstr id) 
{
	iterator it = find(id);
	return (*it).value();
}

bool Id3v2Tag::valid() 
{
	return m_pTagHdr->valid();
}