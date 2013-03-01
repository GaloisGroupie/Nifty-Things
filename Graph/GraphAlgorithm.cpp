/* 
 * File:   GraphAlgorithm.cpp
 * Author: stevenle
 * 
 * Created on December 6, 2012, 6:44 PM
 * 
 * Class containing graph algorithms. All of its methods should be
 * static.
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

/**
 * Takes in prebuilt graph and runs Dijkstra's algorithm on every single node
 * It then sums up all of the the time it takes for each one's ideal path.
 * The weight of each node in the graph is changed according to the algorithm
 * @param g
 * @return 
 */
unsigned long long GraphAlgorithm::getTotalTimeWeight(Graph &g) {
  //Loop overall vertices, do dijkstra on each and sum up total time each time
  std::map<std::string, Vertex*>::iterator verticesIter =
          g.getVertexIterator();

  unsigned long long totalTimeCost = 0;
  std::map<Vertex*, Vertex*> dummyMap;
  while (verticesIter != g.getVertexIteratorEnd()) {

    GraphAlgorithm::dijkstrasAlgorithm(g, (*verticesIter).second, dummyMap);

    //Loop over weight of each node of the graph. It should be minimized
    std::map<std::string, Vertex*>::iterator dijkstraVerticesIter =
            g.getVertexIterator();

    while (dijkstraVerticesIter != g.getVertexIteratorEnd()) {

      totalTimeCost += ((*dijkstraVerticesIter).second)->getWeight();

      dijkstraVerticesIter++;
    }

    verticesIter++;
  }

  return totalTimeCost;
}

/**
 * Takes in a graph, a vertex to start at, and a map where the key is the node
 * we're interested in getting to and its value is the previous value we 
 * should have to take the most optimal path. This algorithm doesn't use
 * Dijkstra's algorithm exactly but instead does a pseudo-inbetween 
 * Dijkstra and Bellman-Ford
 * @param g Constructed graph, can be directed or nondirected. Edges must all be
 * non negative. In addition vertices in the graph will be manipulated.
 * @param rootVertex vertex to start our Dijkstra's algorithm at
 * @param prevMap a map to store previous values for shortest paths
 * @return 
 */
void GraphAlgorithm::dijkstrasAlgorithm(Graph &g, Vertex* rootVertex, std::map<Vertex*,
        Vertex*> &prevMap) {

  Graph djikstraGraph(g.isDirected());

  //Will serve as priority queue for djikstra's
  std::set<std::pair<int, Vertex*> > vertexQueue;

  //Make copy of all vertices, set weight equal to 'INFINITY' in this case
  std::map<std::string, Vertex*>::iterator vertexIter = g.getVertexIterator();

  while (vertexIter != g.getVertexIteratorEnd()) {
    ((*vertexIter).second)->updateWeight(std::numeric_limits<int>::max());

    vertexIter++;
  }

  //Set weight(distance) for rootVertex to be zero
  rootVertex->updateWeight(0);

  vertexQueue.insert(std::make_pair(rootVertex->getWeight(), rootVertex));
  while (!vertexQueue.empty()) {
    int weight = vertexQueue.begin()->first;
    Vertex* vRef = vertexQueue.begin()->second;
    vertexQueue.erase(vertexQueue.begin());

    std::list<Edge*>::iterator edgeIter = vRef->getEdgeIterator();
    while (edgeIter != vRef->getEdgeIteratorEnd()) {

      Vertex* vNeighbor = (*edgeIter)->getEndNode();
      int potentialWeight = weight + (*edgeIter)->getTimeWeight();

      if (potentialWeight < vNeighbor->getWeight()) {
        vertexQueue.erase(std::make_pair(vNeighbor->getWeight(),
                vNeighbor));

        vNeighbor->updateWeight(potentialWeight);
        prevMap[vNeighbor] = vRef;
        vertexQueue.insert(std::make_pair(vNeighbor->getWeight(),
                vNeighbor));
      }

      edgeIter++;
    }
  }

}

/**
 * Takes a connected graph and returns a MST of it
 * @param g constructed graph. can be directed or undirected
 * @return a MST of the graph passed in as an argument
 */
Graph GraphAlgorithm::primsAlgorithm(Graph &g) {
  Graph mst(g.isDirected());
  g.unsetAllVisited();
  //Make a copy of all the vertices but do modified copy since edges
  //will be different. So just make copy of names. Also make a map to keep
  //track if node has been visited yet.
  std::map<std::string, Vertex*>::iterator vertexIter = g.getVertexIterator();
  while (vertexIter != g.getVertexIteratorEnd()) {
    Vertex* v = new Vertex((*vertexIter).first);
    mst.addVertex(v);

    vertexIter++;
  }

  //Create a priority queue of all the edges where priority is the money
  //cost. The lower the money cost, the higher the priority
  std::priority_queue<Edge*, std::vector<Edge*>, Edge::PtrComp> edgeQueue;

  //Push edges into the queue, set first node as being visited as true
  std::list<Edge*>::iterator edgeIter = g.getEdgeIterator();

  std::list<Edge*>::iterator startIter =
          ((*edgeIter)->getStartNode())->getEdgeIterator();
  std::list<Edge*>::iterator endIter =
          ((*edgeIter)->getStartNode())->getEdgeIteratorEnd();

  while (startIter != endIter) {
    edgeQueue.push(*startIter);
    startIter++;
  }

  ((*edgeIter)->getStartNode())->setVisited();

  Edge* e;
  //Actual Prim's algo going through graph and construct MST
  while (!edgeQueue.empty()) {
    e = edgeQueue.top();
    edgeQueue.pop();

    if (!e->getEndNode()->isVisited()) {
      e->getEndNode()->setVisited();

      //Make new copy of edge particular to mst
      Vertex* startNodeMst = mst.getVertex((e->getStartNode()->getName()));
      Vertex* endNodeMst = mst.getVertex((e->getEndNode()->getName()));
      int moneyWeightMst = e->getMoneyWeight();
      int timeWeightMst = e->getTimeWeight();
      Edge* eMst = new Edge(startNodeMst, endNodeMst, moneyWeightMst,
              timeWeightMst);

      mst.addEdge(eMst);

      startIter = (e->getEndNode())->getEdgeIterator();
      endIter = (e->getEndNode())->getEdgeIteratorEnd();
      while (startIter != endIter) {
        edgeQueue.push(*startIter);
        startIter++;
      }
    }
  }

  return mst;
}

/**
 * Goes through all the edges of the graph and sums up their money cost
 * If the graph passed in as an argument is undirected, then the value is 
 * divided by 2 since we overcount by 2
 * @param g constructed graph
 * @return 
 */
unsigned long long GraphAlgorithm::getTotalMoneyWeight(Graph &g) {
  unsigned long long TotalMoneyWeight = 0;

  std::list<Edge*>::iterator edgeIter = g.getEdgeIterator();
  while (edgeIter != g.getEdgeIteratorEnd()) {
    TotalMoneyWeight += (*edgeIter)->getMoneyWeight();
    edgeIter++;
  }

  //If not directed, divide by 2 since overcounted twice
  if (!g.isDirected()) {
    TotalMoneyWeight = TotalMoneyWeight / 2;
  }

  return TotalMoneyWeight;
}
