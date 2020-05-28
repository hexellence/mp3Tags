// mp3Stuff.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include"Id3v2Frm.h"


using namespace std;

const char filePath[] = { "D:\\mp3\\mp3\\Alberto Iglesias - Banda\\Habla Con Ella\\01 - Habla Con Ella - Track 1.mp3" };


int main()
{

    Id3v2Tag tag1(filePath);
    //Id3v2Tag tag2(filePath);
    //Id3v2Tag tag3(filePath);

    //TAG1
    cout << "check iterator" << endl;
    for (Id3v2Tag::iterator it = tag1.begin(); it != tag1.end(); it++) {
        cout << (*it).id() << ": " << (*it).value() << endl;
    }

    cout << endl;

    //insert
    cout << "insert COMM in place of TIT2" << endl;
    tag1.insert(tag1.find("TIT2"), "COMM", "Deniz Erdogan");
    
    for (Id3v2Tag::iterator it = tag1.begin(); it != tag1.end(); it++) {
        cout << (*it).id() << ": " << (*it).value() << endl;
    }

    cout << endl;

    //del method
    cout << "deleteing MCDI, GEOB, TIT2" << endl;
    tag1.del("MCDI");
    tag1.del("GEOB");
    tag1.del("TIT2");
    for (Id3v2Tag::iterator it = tag1.begin(); it != tag1.end(); it++) {
        cout << (*it).id() << ": " << (*it).value() << endl;
    }

    cout << endl;

    //clear
    cout << "clear method" << endl;
    tag1.clear();
    for (Id3v2Tag::iterator it = tag1.begin(); it != tag1.end(); it++) {
        cout << (*it).id() << ": " << (*it).value() << endl;
    }

    cout << endl;

    tag1.push_back("TIT2", "Tunca Erdogan");
    tag1.push_back("TALB", "Deniz Erdogan");
    tag1.push_back("COMM", "Tunca Erdogan");
    for (Id3v2Tag::iterator it = tag1.begin(); it != tag1.end(); it++) {
        cout << (*it).id() << ": " << (*it).value() << endl;
    }

    std::cout << "Hello World!\n";
}