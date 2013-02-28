/* 
 * File:   BitOutputStream.cpp
 * Author: cs100fax, Steven Tuan Le
 * 
 * Created on November 10, 2012, 5:58 PM
 */

#include "BitOutputStream.hpp"
#include <fstream>

BitOutputStream::BitOutputStream(std::ostream & os) : out(os) {
    buf = nbits = bytesWritten = 0;
}

/**
 * Flushes the remaining bits in the buffer
 */
void BitOutputStream::flush() {
    bytesWritten++;
    out.put(buf);
    out.flush();
    buf = nbits = 0;
}

/** Write the least significant bit of the argument to 
 * the bit buffer, and increment the bit buffer index.
 * But flush the buffer first, if it is full. 
 */
void BitOutputStream::writeBit(int i) {

    int mask = 1; //Create mask to get least significant bit
    int desiredBit = i & mask; //If least sig is 1, get 1. If 0 get 0;

    buf = (buf << 1) | desiredBit; //Shift bits one to left. Write desired
    nbits++; //Increase number of bits holding

    if (nbits == 8) {
        BitOutputStream::flush();
    }
}

/**
 * Flushes any remaining bytes in the buffer
 * @return returns number of stray bits
 */
int BitOutputStream::finish() {
    if (nbits > 0) {
        int bitsRef = nbits;
        buf = buf << (8 - nbits);
        BitOutputStream::flush();
        return bitsRef;
    }
    return 0;
}

int BitOutputStream::getBytesWritten() {
    return bytesWritten;
}

