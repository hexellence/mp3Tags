// mp3Stuff.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include"Id3v2Frm.h"


using namespace std;

const char filePath[] = { "D:\\mp3\\mp3\\Alberto Iglesias - Banda\\Habla Con Ella\\01 - Habla Con Ella - Track 1.mp3" };


int main()
{

    Id3v2Tag myTag(filePath);

    for (Id3v2Tag::iterator it = myTag.begin(); it != myTag.end(); it++) {
        cout << (*it).id() << endl;
    }

    myTag.del("GEOB");
    myTag.del("TIT2");
    myTag.del("TCON");
    myTag.del("TRCK");
    myTag.del("MCDI");
    myTag.del("TFLT");
    myTag.del("TALB");
    myTag.del("COMM");
    myTag.del("PRIV");
    myTag.del("PRIV");
    myTag.del("PRIV");
    myTag.del("PRIV");
    myTag.del("PRIV");
    myTag.del("PRIV");
    myTag.del("TYER");
    myTag.del("TPE2");
    myTag.del("TCOM");
    myTag.del("TPE1");

    myTag.append("TIT2", "Tunca Erdogan");
    myTag.append("TALB", "Deniz Erdogan");
    
    

    

    myTag.append("TIT2", "Tunca Erdogan");

    //Id3v2Tag tag(filePath);

    //cout << tag["TIT2"] << endl;

    //tag["TIT2"] = "Deneme";

    //for (Id3v2Tag::iterator it = tag.begin(); it != tag.end(); it++)
    //{
    //    //cout << (*it).getFrameId() << endl;
    //}


    std::cout << "Hello World!\n";
}