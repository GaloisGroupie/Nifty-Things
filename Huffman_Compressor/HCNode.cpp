/* 
 * File:   HCNode.cpp
 * Author: cs100fax, Steven Tuan Le
 * 
 * Created on November 10, 2012, 5:27 PM
 */

#include "HCNode.hpp"

bool HCNode::operator<(const HCNode& other) {
    //If counts are different, just compare counts
    if (count != other.count) {
        return count > other.count;
    }

    //If counts are equal, use symbol value to break tie
    return symbol < other.symbol;
}

