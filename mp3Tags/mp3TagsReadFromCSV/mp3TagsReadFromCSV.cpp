
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "hxlstr.h"
#include "csvFileOp.h"
#include "ID3.h"

using namespace std;
using namespace hxl;

const char input_file_name[] = { "D:\\TUNCA_3_music\\musicArchiveEdit.csv" };



void readFileToVector(ifstream& file, vector<vector<hxlstr>>& csv) {       
    
    if (!file) {
        std::cout << "File opening failed\n";
    }
    else {
        //file is ready
        char charPair[3]{};
        file.read(charPair, 2);
        hxlstr newChar = charPair;
        hxlstr word;                    

        //check BOM
        if (newChar == BOM) {
            vector<hxlstr> column;
            while (!file.eof()) 
            {
                file.read(charPair, 2);                  
                newChar = charPair;
                if (newChar == CR) 
                {
                    file.read(charPair, 2); //dummy read
                    column.push_back(word);
                    csv.push_back(column);
                    word = "";
                    column.clear();
                    
                }
                else if (newChar == SEPARATOR) 
                {
                    column.push_back(word);
                    word = "";                    
                }
                else 
                {
                    word = word + newChar;
                }
            }//do until the eof
        } //if there is BOM

    } // file ready    
}




int main()
{

    ifstream csvFile;    
    ofstream mp3File;
    csvFile.open(input_file_name, std::ios::binary | std::ios::beg);
    vector<vector<hxlstr>> csv;

    
    readFileToVector(csvFile, csv);




    mp3File.open((char16_t*)csv[1][5].c_str(), std::ios::binary | std::ios::beg); 
   

    

    std::cout << "Hello World!\n";
}