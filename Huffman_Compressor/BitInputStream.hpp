/* 
 * File:   BitInputStream.hpp
 * Author: stevenle
 *
 * Created on November 10, 2012, 5:58 PM
 */

#ifndef BITINPUTSTREAM_HPP
#define	BITINPUTSTREAM_HPP

#include <fstream>

class BitInputStream {
public:

    /** Initialize a BitInputStream that will use 
     * the given istream for input. 
     */
    BitInputStream(std::istream & is);

    /** Fill the buffer from the input */
    void fill();

    /** Read the next bit from the bit buffer.
     * Fill the buffer from the input stream first if needed.
     * Return 1 if the bit read is 1;
     * return 0 if the bit read is 0.  
     * 
     */
    int readBit();

    /** Wrapper to see if input is good
     */
    bool good();

    /** Shifts the buffer to compensate for number of trailing bits
     */
    void shift(int trailingBits);
private:
    char buf; //one byte buffer of bits
    int nbits; //how many bits have been read from buf
    std::istream & in; //the input stream to use

};

#endif	/* BITINPUTSTREAM_HPP */

