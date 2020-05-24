// mp3Stuff.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include"Mp3InfoTag.h"
#include"Id3v2Def.h"

using namespace std;

const char filePath[] = { "D:\\mp3\\mp3\\Alberto Iglesias - Banda\\Habla Con Ella\\01 - Habla Con Ella - Track 1.mp3" };


int main()
{
    ifstream mp3File;
    char tagBuffer[1000];

    mp3File.open(filePath, std::ios::binary);
    mp3File.seekg(0, std::ios::beg);
    mp3File.read((char*)tagBuffer, 100);


    Mp3InfoTag tag;
    Mp3InfoFrm album("TALB", "Tunca");
    Mp3InfoFrm title("TIT2", "Denizaki");
    Mp3InfoFrm year("TYER", "1977");

    tag.add(album);
    tag.add(title);
    tag.add(year);


    for(auto item:tag)
    {
        cout << item.payload() << endl;
    }
    
    ID3V2TAGHDR taghdr;
    
    int tagSize = getID3v2TagHeader(filePath, &taghdr);

    char* pWholeTag = new char[tagSize];

    ID3V2FRMHDR* currentFrame = readID3v2Tag(filePath, pWholeTag, tagSize);

    while (getNextID3v2Frame(currentFrame) != nullptr) {
        tag.add(Mp3InfoFrm(*currentFrame));
    }
    

    std::cout << "Hello World!\n";
}