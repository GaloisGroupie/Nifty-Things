#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include <string>
#include "HCNode.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:

    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        return *lhs < *rhs;
    }
};

/** A Huffman Code Tree class.
 *  Not very generic:  Use only if alphabet consists
 *  of unsigned chars.
 */
class HCTree {
private:
    HCNode* root;
    HCNode* traverseNode;
    vector<HCNode*> leaves;
    int bytesWritten;
    int trailingBits;
    int numChars;

    void calcEncodedValues(const vector<HCNode*> & leaves,
        vector<string> &encodedValues);
    void calcEncodedNodeValue(HCNode *encodedNode,
        vector<string> &encodedValues, int index);
    void deleteAllHelper(HCNode* root);
public:
    vector<string> encodedValues;

    explicit HCTree() : root(0) {
        leaves = vector<HCNode*>(256, (HCNode*) 0);
        encodedValues = vector<string > (256, "");
        bytesWritten = 0;
        trailingBits = 0;
        numChars = 0;
    }

    ~HCTree();

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void build(const vector<int>& freqs);

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int decode(BitInputStream& in, std::ofstream& outFile);


    /** Flushes any stray bits remaining
     *  PRECONDITION: nbits > 0
     */
    void finish(BitOutputStream & out);

    /** Creates header for Huffman
     */
    void createHeader(std::ofstream & outFile, std::vector<int> & Counter,
    int & trailingBits, double & fileSize);

    void extractHeader(std::ifstream & inFile, std::vector<int> & Counter,
        int & trailingBits, double & fileSize);

    /** Resets bytesWritten and trailingBits to 0
     */
    void resetCounters();

    /** Set bytes written and trailing bits equal to instance value
     */
    void getCounters(int & bytesWrittenC, int & trailingBitsC);
    
    /** Does post order traversal to delete all leaves
     */
    void deleteAll(HCNode* root);
};

#endif // HCTREE_HPP
