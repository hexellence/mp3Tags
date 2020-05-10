// hxlString.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "hxlstr.h"
#include "trim.h"

using namespace std;
using namespace hxl;

int main()
{
    char test8str[] = "$$$$$Tu$nc$$$$$";
    
    hxlstr test1 = test8str;
    hxlstr test2 = test8str;
    hxlstr test3 = test8str;
    
    test1.rtrim(u"$");
    test2.ltrim(u"$");
    test3.trim(u"$");
   // trim_both(test8str, sizeof(test8str), " ");
    
    

    std::cout << "Hello World!\n";
}
