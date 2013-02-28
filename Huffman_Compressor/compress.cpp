/* 
 * File:   main.cpp
 * Author: cs100fax, Steven Tuan Le
 *
 * Created on November 10, 2012, 4:28 PM
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "HCTree.hpp"
#include <cstdlib>


void determineFrequency(char * fileName, std::vector<int> & Counter);
void writeCompressedFile(char * inputFile, char * outputFile, HCTree & hc,
        std::vector<int> & Counter, double fileSize);

int main(int argc, char *argv[]) {

    std::vector<int> HCCounter(256, 0);

    //HCCounter is passed by reference so we can get a counter for
    //the frequencies of the bytes
    determineFrequency(argv[1], HCCounter);

    //Get input file size
    int start, end;
    std::ifstream inFile;
    inFile.open(argv[1]);
    start = inFile.tellg();
    inFile.seekg(0, ios::end);
    end = inFile.tellg();
    inFile.close();
    double uncompFileSize = end-start;
    
    int charCounter = 0;
    for(int m = 0; m < 256; m++) {
        if(HCCounter[m] != 0)
            charCounter++;
    }
    
    std::cout << "Found " << charCounter << " unique symbols in input file ";
    std::cout << "of size " << (int) uncompFileSize << " bytes." << std::endl;
    
    //Construct Huffman coding tree
    HCTree hc;
    std::cout << "Building Huffman code tree... ";
    hc.build(HCCounter);
    std::cout << "done." << std::endl;

    //Begin compression
    writeCompressedFile(argv[1], argv[2], hc, HCCounter, uncompFileSize);

    //Get compressed file size
    inFile.open(argv[2]);
    start = inFile.tellg();
    inFile.seekg(0, ios::end);
    end = inFile.tellg();
    inFile.close();
    double compFileSize = end-start;
    
    std::cout << "Output file has size " << (int) compFileSize << " bytes." 
            << std::endl;
    std::cout << "Compression ratio: " 
            << (compFileSize/uncompFileSize) << std::endl;
    
    return 0;

}

/**
 * Main method to create the compressed file
 * @param inFileName reference to file to be compressed
 * @param outFileName reference to where compressed file to be written
 * @param hc constructed huffman tree/encoding for the file
 * @param HCCounter frequency counter for bytes used in file
 */
void writeCompressedFile(char * inFileName, char * outFileName, HCTree & hc,
        std::vector<int> & HCCounter, double fileSize) {
    std::ifstream inputFile, dummyInputFile;
    inputFile.open(inFileName, std::ios::binary);
    dummyInputFile.open(inFileName, std::ios::binary);
    std::ofstream outputFile;
    outputFile.open(outFileName, std::ios::binary);


    char ch;
    if (inputFile.is_open() && outputFile.is_open()) {
        BitOutputStream outWriter(outputFile);

        /** This block here is to just determine the number of bytes that 
         *  will be written and the number of trailing byts
         */
        //Write the compressed file. Use dummy to get num of trailing bits
        while (dummyInputFile.good()) {
            ch = (char) dummyInputFile.get();
            hc.encode(ch, outWriter);
            
            dummyInputFile.peek();
        }

        //Flush out any remaining bits and get number of trailing bits
        hc.finish(outWriter);

        int bytesWritten = 0;
        int trailingBits = 0;
        hc.getCounters(bytesWritten, trailingBits);
        hc.resetCounters();


        std::cout << "Writing to file \"" << outFileName << "\"... ";
        /** This block here is to legitimately write the compressed data
         *
         * 
         */
        //Write out bytes and their frequency for decompression
        std::ofstream realOutputFile;
        realOutputFile.open(outFileName, std::ios::binary);
        hc.createHeader(realOutputFile, HCCounter, trailingBits, fileSize);
        BitOutputStream realOutWriter(realOutputFile);
        //Write the compressed file.
        while (inputFile.good()) {
            ch = (char) inputFile.get();
            hc.encode(ch, realOutWriter);
            
            inputFile.peek();
        }
        //Flush out any remaining bits and get number of trailing bits
        hc.finish(realOutWriter);
        std::cout << "done." << std::endl;
    } else {
        std::cout << "File could not be opened" << std::endl;
        inputFile.close();
        exit(0);
    }

    inputFile.close();
    outputFile.close();
}

/**
 * Opens a file and determines the frequency of all the bytes
 */
void determineFrequency(char * fileName,
        std::vector<int> & Counter) {

    std::ifstream inputFile;
    inputFile.open(fileName, std::ios::binary);
    int ch;

    //Read in data and store bytes and frequency of it in HCCounter
    //which is map implementation
    if (inputFile.is_open()) {
        std::cout << "Reading from file \"" << fileName << "\"... ";
        while (inputFile.good()) {
            ch = inputFile.get();

            if (ch >= 0) {
                Counter[ch] = Counter[ch] + 1;
            }
        }
        std::cout << "done." << std::endl;
    } else {
        std::cout << "File could not be opened" << std::endl;
        inputFile.close();
        exit(0);
    }

    inputFile.close();
}

