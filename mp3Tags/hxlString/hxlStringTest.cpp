// hxlString.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "hxlstr.h"

using namespace std;

int main()
{
    char test8str[]{ "Naber Tunca Nasil gidiyor" };
    char16_t test16str[]{ u"Naber Tunca Nasil gidiyor" };
    const uint8_t* p_testBuffer = (uint8_t*)&test16str;



    hxlstr test1 = "Tunca";
    hxlstr test2 = u"Tunca";
    hxlstr text3 = "";

    if (test1 == test2) {
        cout << "texts are equal" << endl;
    }


    



    std::cout << "Hello World!\n";
}
