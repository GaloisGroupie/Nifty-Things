/* 
 * File:   main.cpp
 * Author: Steven Le (cs100fax)
 *
 * Created on December 4, 2012, 2:20 PM
 */

#include <cstdlib>
#include <string>
#include "Vertex.hpp"
#include "Graph.hpp"
#include "Edge.hpp"
#include "GraphAlgorithm.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <queue>
#include <limits>
#include <set>
#include <cmath>

/*
 * Main can do two different things depending on the size of the graph,
 * particularly the number of edges. If the number of edges in the graph is
 * greater than 20, then the first MST found will be printed along with
 * all the other stuff such as its money weight, time weight etc.
 * 
 * If there is less than/equal to 20 edges, the MST minimizing the time
 * weight is returned. This is found in exponential time by essentially
 * finding all the edges belonging to cycles and then trying all
 * possible removals of them and seeing, if the graph is connected, which
 * MST takes the least time.
 */
int main(int argc, char* argv[]) {

  Graph originalGraph(false);
  Graph::createGraph(originalGraph, argv[1]);

  if (argc < 2) {
    std::cout << argv[0] << " called with incorrect arguments" << std::endl;
    return 1;
  }

  if (Graph::graphIsConnected(originalGraph)) {

    Graph mst = GraphAlgorithm::primsAlgorithm(originalGraph);

    unsigned long long originalMoneyCost =
            GraphAlgorithm::getTotalMoneyWeight(originalGraph);

    unsigned long long mstMoneyCost = GraphAlgorithm::getTotalMoneyWeight(mst);
    unsigned long long originalTimeCost = GraphAlgorithm::getTotalTimeWeight(originalGraph);
    unsigned long long mstTimeCost = GraphAlgorithm::getTotalTimeWeight(mst);

    //If has less than 20 edges do something special ;D
    if (originalGraph.getNumOfEdges() <= 20) {

      //Copy all edges into this list to test out potentially removable
      //edges.
      std::vector<Edge*> copyList;

      std::list<Edge*>::iterator edgeIter =
              originalGraph.getEdgeIterator();

      while (edgeIter != originalGraph.getEdgeIteratorEnd()) {

        //If the edge is not a mirror edge we already have, add it
        std::vector<Edge*>::iterator mirrorCheck;
        mirrorCheck = std::find(copyList.begin(), copyList.end(),
                (*edgeIter)->getMirrorEdge());

        //If mirror node is not in the list, add the node.
        if (mirrorCheck == copyList.end())
          copyList.push_back(*edgeIter);

        edgeIter++;
      }

      //We now have a reference to all the edges of the graph inside
      //copyList. Test which edges, when removed still keep the graph
      //connected.
      std::vector<Edge*> removableEdges;
      while (!copyList.empty()) {
        Edge* edgeRef = copyList.back();
        copyList.pop_back();

        originalGraph.deleteEdge(edgeRef);

        if (Graph::graphIsConnected(originalGraph))
          removableEdges.push_back(edgeRef);

        //Re add edge into the graph
        originalGraph.addEdge(edgeRef, edgeRef->getMirrorEdge());
      }

      //Now we make a power set of edges we can remove and test each
      //one. If removing the edges in a certain set gives an MST, see
      //if the time cost is less. If it is less, that is our new MST
      //To get power set, we will use bit shifting on integers. Since
      //The number of edges is so low, using a regular int will suffice
      int numSet = (int) std::pow(2.0, (int) removableEdges.size());

      //Keep track of edges we removed so we can simply reinsert after
      //we are done.
      std::vector<Edge*> removedEdges;
      int mask = 1;
      for (int i = 1; i < numSet; i++) {

        //Get power set to test out
        int copy = i;
        for (int bit = 0; bit < removableEdges.size(); bit++) {
          if ((copy & mask) == 1) {
            removedEdges.push_back(copyList[bit]);
          }
          copy = copy >> 1;
        }

        //Remove all edges in the power set
        for (int j = 0; j < removedEdges.size(); j++) {
          originalGraph.deleteEdge(removedEdges[j]);
        }

        //If the graph is still connected, get an MST and compare it
        //to the current best one. If it's less time, then set the
        //new MST as the best MST
        if (Graph::graphIsConnected(originalGraph)) {
          Graph newMst = GraphAlgorithm::primsAlgorithm(originalGraph);
          unsigned long long newMstMoneyCost = GraphAlgorithm::getTotalMoneyWeight(newMst);

          if (newMstMoneyCost == mstMoneyCost) {
            unsigned long long newMstTimeCost = GraphAlgorithm::getTotalTimeWeight(newMst);

            if (newMstTimeCost < mstTimeCost) {

              mstMoneyCost = newMstMoneyCost;
              mstTimeCost = newMstTimeCost;

            }
          }
        }

        //Restore the graph and clear removedEdges
        for (int j = 0; j < removedEdges.size(); j++) {
          originalGraph.addEdge(removedEdges[j], removedEdges[j]->getMirrorEdge());
        }
        removedEdges.clear();
      }
    }

    //Print out the costs and everything

    std::cout << originalMoneyCost << std::endl;
    std::cout << mstMoneyCost << std::endl;
    std::cout << originalMoneyCost - mstMoneyCost << std::endl;
    std::cout << originalTimeCost << std::endl;
    std::cout << mstTimeCost << std::endl;
    std::cout << (mstTimeCost - originalTimeCost) << std::endl;


  } else {
    std::cout << "Graph is not connected" << std::endl;
  }

}