// mp3Stuff.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include"Id3v2Tag.h"


using namespace std;

const char filePath[] = { "D:\\mp3\\mp3\\Alberto Iglesias - Banda\\Habla Con Ella\\01 - Habla Con Ella - Track 1.mp3" };


int main()
{

    Id3v2Tag tag(filePath);

    //cout << tag["TIT2"] << endl;

    tag["TIT2"] = "Deneme";

    //for (Id3v2Tag::iterator it = tag.begin(); it != tag.end(); it++)
    //{
    //    //cout << (*it).getFrameId() << endl;
    //}
    

    std::cout << "Hello World!\n";
}