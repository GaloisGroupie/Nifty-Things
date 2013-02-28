/* 
 * File:   BitInputStream.cpp
 * Author: cs100fax, Steven Tuan Le
 * 
 * Created on November 10, 2012, 5:58 PM
 */

#include "BitInputStream.hpp"

BitInputStream::BitInputStream(std::istream & is) : in(is) {
    buf = 0;
    nbits = 8;
}

/**
 * Fills buffer with 8 bits to read
 */
void BitInputStream::fill() {
    buf = in.get();
    nbits = 8;
}

/**
 * Changes number of bits to read to be trailingBits number
 * @param trailingBits number of bits to read from the buffer
 */
void BitInputStream::shift(int trailingBits) {
    if (trailingBits != 8)
        nbits = trailingBits;
}

/**
 * Reads the bits in the buffer. Once all bits read, -1 is returned
 * @return 1/0 for valid read. -1 for invalid read 
 */
int BitInputStream::readBit() {
    if (nbits == 0) {
        return -1;
    }

    int mask = 256;
    if ((mask & buf) == 256) {
        nbits--;
        buf = buf << 1;
        return 1;
    } else if ((mask & buf) == 0) {
        nbits--;
        buf = buf << 1;
        return 0;
    }

    return -1;
}

/**
 * Wrapper method to see if instream is still good
 * @return true if no eof, badbit, errorbit flags sent for ifstream
 */
bool BitInputStream::good() {
    return in.good();
}


