/* 
 * File:   uncompress.cpp
 * Author: cs100fax, Steven Tuan Le
 * 
 * Created on November 11, 2012, 5:47 PM
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "HCTree.hpp"
#include <cstdlib>

void testRead(char * inputFile);

int main(int argc, char *argv[]) {

    std::ifstream inputFile;
    inputFile.open(argv[1], std::ios::binary);

    HCTree hc;
    int trailingBits = 0;
    double fileSize = 0;
    std::vector<int> HCCounter(256, 0);

    std::cout << "Reader header from file \"" << argv[1] << "\"... ";
    hc.extractHeader(inputFile, HCCounter, trailingBits, fileSize);
    std::cout << "done." << std::endl;
    
    int charCount = 0;
    for(int m = 0; m < 256; m++) {
        if(HCCounter[m] != 0)
            charCount++;
    }
    
    std::cout << "Uncompressed file will have " << charCount << " unique ";
    std::cout << "symbols and size " << (int) fileSize << " bytes." << std::endl;
    
    std::cout << "Building Huffman code tree... ";
    hc.build(HCCounter);
    std::cout << "done." << std::endl;

    std::ofstream outputFile;
    outputFile.open(argv[2], std::ios::binary);

    BitInputStream inStream(inputFile);

    char buf;
    int n = 0;
    std::cout << "Writing to file \"" << argv[2] << "\"... ";
    while (inStream.good()) {
        n++;
        inStream.fill();
        if (trailingBits != 0) {
            inputFile.peek();
        }
        /* If it's no longer good, or we're on the last byte, we will do some
         * shifting to properly extract encoding
         */

        if (!inStream.good()) {
            inStream.shift(trailingBits);
            hc.decode(inStream, outputFile);
            break;
        }

        hc.decode(inStream, outputFile);
    }
    std::cout << "done." << std::endl;
    
    //Get compressed file size
    int start, end;
    std::ifstream inFile;
    inFile.open(argv[1]);
    start = inFile.tellg();
    inFile.seekg(0, ios::end);
    end = inFile.tellg();
    inFile.close();
    double compFileSize = end-start;
    
    std::cout << "Uncompression ratio: " << fileSize/compFileSize
            << std::endl;

    return 0;
}