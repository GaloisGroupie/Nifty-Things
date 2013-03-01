/* 
 * File:   Vertex.hpp
 * Author: Steven Le (cs100fax)
 *
 * Created on December 4, 2012, 2:23 PM
 * 
 * Each vertex class has a name, adjacencylist, and a weight. The name could
 * be anything but in this case, it is for webservers. Names must be unique
 * because they the keys for a hash map used by the graph. The weight could
 * be anything and there is no constraint on what it could represent. In this
 * assignment, the weight is used to represent the total distance in 
 * Dijkstra's algorithm.
 */

#ifndef VERTEX_HPP
#define	VERTEX_HPP

#include <iostream>
#include <list>
#include <string>
#include "Edge.hpp"

class Vertex {
public:

  /**
   * By default if no second argument is given, the weight variable is
   * set to be 0
   * @param name
   */
  Vertex(std::string name) : name(name) {
    weight = 0;
    visited = false;
  }

  Vertex(std::string name, int weight) : name(name), weight(weight) {
    visited = false;
  }

  void addEdge(Edge*);
  std::list<Edge*>::iterator getEdgeIterator();
  std::list<Edge*>::iterator getEdgeIteratorEnd();
  void updateWeight(int newWeight);
  int getWeight();

  std::string getName();

  struct PtrComp {

    bool operator()(Vertex *one, Vertex * other) {
      return one->getWeight() > other->getWeight();
    }
  };

  bool isVisited();
  void setVisited();
  void unsetVisited();
  void deleteEdge(Edge* e);
private:
  std::string name;
  std::list<Edge*> adjacencyList;
  int weight;
  bool visited;
};

#endif	/* VERTEX_HPP */

