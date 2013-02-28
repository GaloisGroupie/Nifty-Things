/* 
 * File:   HCTree.cpp
 * Author: cs100fax, Steven Tuan Le
 * 
 * Created on November 10, 2012, 5:28 PM
 */

#include "HCTree.hpp"
#include <queue>
#include <string>
#include <cstdlib>

/**
 * Struct to store header information. Write binary encoding of it as header
 */
struct HuffmanHeader {
    int frequency[256];
    int trailingBits;
    double fileSize;
};

/**
 * For destructor, make sure to delete all node objects that leaves point
 * to
 */
HCTree::~HCTree() {
    if(numChars != 0)
        HCTree::deleteAll(root);
    
    for(int i = 0; i < 256; i++) {
        leaves[i] = 0;
    }
    traverseNode = 0;
}

/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs) {
    std::priority_queue<HCNode*, vector<HCNode*>, HCNode::PtrComp> forestQueue;

    //Iterate through every single byte. If it has frequency more
    //than one, create a node and add it to priority queue
    char charRef;
    numChars = 0;
    for (int i = 0; i < 256; i++) {
        if (freqs[i] > 0) {
            numChars++;
            HCNode *hcNode = new HCNode(freqs[i], i);
            leaves[i] = hcNode;
            charRef = i;
            forestQueue.push(hcNode);
        }
    }

    //Pop in empty dummy node in case only 1 letter
    if (numChars == 1) {
        forestQueue.push(new HCNode(-1, 0));
    }

    //Create Huffman Tree
    HCNode *nodeRef1, *nodeRef2, dummyRef;
    while (!forestQueue.empty()) {
        nodeRef1 = forestQueue.top();
        forestQueue.pop();

        //If forestQueue is now empty, it means HCTree complete
        if (forestQueue.empty()) {
            break;
        }

        nodeRef2 = forestQueue.top();
        forestQueue.pop();

        HCNode* parentNode = new HCNode(nodeRef1->count + nodeRef2->count, 'a');
        nodeRef1->p = parentNode;
        nodeRef2->p = parentNode;

        //We put node with higher count on the left
        if (*nodeRef1 < *nodeRef2) {
            parentNode->c0 = nodeRef1;
            parentNode->c1 = nodeRef2;

            //Put this new node back in the priority queue
            forestQueue.push(parentNode);
        } else {
            parentNode->c0 = nodeRef2;
            parentNode->c1 = nodeRef1;

            //Put this new node back in the priority queue
            forestQueue.push(parentNode);
        }
    }

    root = nodeRef1;
    traverseNode = root;

    //Call calcEncodedValues method to get Huffman encoded value
    calcEncodedValues(leaves, encodedValues);

}

/**
 * Traverses the Huffman trie and stores all encoded values in encodedValues
 * @param leaves vector to all huffman nodes
 * @param encodedValues encoded values. Ex (char) 0's encoding is at
 * encodedValues[0]
 */
void HCTree::calcEncodedValues(const vector<HCNode*> &leaves,
        vector<string> &encodedValues) {

    for (int i = 0; i < 256; i++) {
        if (leaves[i] != 0) {
            calcEncodedNodeValue(leaves[i], encodedValues, i);
        }
    }
}

/**
 * Helper method to calculate the encoded node's value
 * @param encodedNode reference to the huffman trie leafe
 * @param encodedValues vector where all encoded values stored
 * @param index char value
 */
void HCTree::calcEncodedNodeValue(HCNode *encodedNode,
        vector<string> &encodedValues, int index) {

    if (encodedNode->p != 0) {

        calcEncodedNodeValue(encodedNode->p, encodedValues, index);
        if (encodedNode == (encodedNode->p)->c0) {
            encodedValues[index] += "0";
        } else {
            encodedValues[index] += "1";
        }
    }


}

/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {

    for (int i = 0; i < encodedValues[symbol].length(); i++) {
        if (encodedValues[symbol][i] == '0') {
            //std::cout << "0";
            out.writeBit(0);
        } else if (encodedValues[symbol][i] == '1') {
            //std::cout << "1";
            out.writeBit(1);
        } else {
            std::cout << "Something with terribly wrong in HCTree encode" <<
                    std::endl;
            exit(1);
        }
    }

}

/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
int HCTree::decode(BitInputStream& in, std::ofstream& outFile) {

    int bitNum = in.readBit();
    if (traverseNode == 0) {
        return 0;
    }

    while (bitNum != -1) {
        if (bitNum == 0) {
            //std::cout << "0";
            if (traverseNode->c0 != 0) {
                traverseNode = traverseNode->c0;
                if (traverseNode->c0 == 0 && traverseNode->c1 == 0) {
                    outFile.put(traverseNode->symbol);
                    traverseNode = root;
                }
            } else {
                outFile.put(traverseNode->symbol);
            }

        } else if (bitNum == 1) {
            //std::cout << "1";
            if (traverseNode->c1 != 0) {
                traverseNode = traverseNode->c1;
                if (traverseNode->c0 == 0 && traverseNode->c1 == 0) {
                    outFile.put(traverseNode->symbol);
                    traverseNode = root;
                }
            } else {
                outFile.put(traverseNode->symbol);
            }
        }

        bitNum = in.readBit();
    }

    return 1;

}

/**
 * Flush out any remaining bits in the buffer
 */
void HCTree::finish(BitOutputStream& out) {
    trailingBits = out.finish();
    bytesWritten = out.getBytesWritten();
}

/**
 * Creates a HuffmanHeader struct and writes binary encoding of it in the 
 * out file
 * @param outFile ofstream to outfile
 * @param Counter vector containing frequency of bytes
 * @param numTrailingBits number of trailing bits
 * @param numBytesWritten number of total bytes written
 */
void HCTree::createHeader(std::ofstream & outFile, std::vector<int> & Counter, 
        int & numTrailingBits, double & fileSize) {
    if (outFile.is_open()) {
        struct HuffmanHeader h;
        for (int i = 0; i < 256; i++) {
            h.frequency[i] = Counter[i];
        }
        h.trailingBits = numTrailingBits;
        h.fileSize = fileSize;

        outFile.write((char *) &h, sizeof (struct HuffmanHeader));
    }
}

/**
 * Extracts HuffmanHeader struct from the input file and copies the frequency
 * number of trailing bits, and number of bytes written over into given 
 * parameters
 */
void HCTree::extractHeader(std::ifstream & inFile, std::vector<int> & Counter,
        int & trailingBits, double & fileSize) {
    if (inFile.is_open()) {
        struct HuffmanHeader h;
        inFile.read((char *) &h, sizeof (struct HuffmanHeader));

        for (int m = 0; m < 256; m++) {
            Counter[m] = h.frequency[m];
        }
        trailingBits = h.trailingBits;
        fileSize = h.fileSize;

    } else {
        std::cout << "Could not open compressed file" << std::endl;
        exit(0);
    }
}

void HCTree::resetCounters() {
    bytesWritten = 0;
    trailingBits = 0;
}

void HCTree::getCounters(int& bytesWrittenC, int& trailingBitsC) {
    bytesWrittenC = bytesWritten;
    trailingBitsC = trailingBits;

}

void HCTree::deleteAll(HCNode* root) {
    HCTree::deleteAllHelper(root);
}

/**
 * Does post order traversal to delete every node onward from the one given
 * as parameter
 */
void HCTree::deleteAllHelper(HCNode* root) {
    if (root == 0) {
        return;
    }
    HCTree::deleteAllHelper(root->c0);
    HCTree::deleteAllHelper(root->c1);
    delete root;
}