/* 
 * File:   BitOutputStream.hpp
 * Author: stevenle
 *
 * Created on November 10, 2012, 5:58 PM
 */

#ifndef BITOUTPUTSTREAM_HPP
#define	BITOUTPUTSTREAM_HPP

#include <fstream>

class BitOutputStream {
public:

    /** Initialize a BitOutputStream that will use 
     * the given ostream for output. 
     */
    BitOutputStream(std::ostream & os);

    /** Send the buffer to the output, and clear it */
    void flush();

    /** Write the least significant bit of the argument to 
     * the bit buffer, and increment the bit buffer index.
     * But flush the buffer first, if it is full. 
     */
    void writeBit(int i);

    /** Flushes any stray bits remaining
     *  PRECONDITION: nbits > 0
     */
    int finish();

    /** Returns number of bytes that have been written/flushed
     */
    int getBytesWritten();

private:
    char buf; //one byte buffer of bits
    int nbits; //number of bits written to buf
    int bytesWritten;
    std::ostream & out; //reference to output stream
};

#endif	/* BITOUTPUTSTREAM_HPP */

