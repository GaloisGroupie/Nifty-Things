/* 
 * File:   Vertex.cpp
 * Author: Steven Le (cs100fax)
 * 
 * Created on December 4, 2012, 3:30 PM
 */

#include "Vertex.hpp"
#include "Edge.hpp"
#include <algorithm>

/**
 * Checks to see if edge trying to be added is valid i.e starting node
 * is this vertex
 * @param e
 */
void Vertex::addEdge(Edge* e) {
  if ((e->getStartNode()) == this)
    adjacencyList.push_back(e);
}

/**
 * The weight is not immutable so the weight of the vertex can be updated
 * as please
 * @param newWeight
 */
void Vertex::updateWeight(int newWeight) {
  weight = newWeight;
}

/**
 * Everything below is just simply getters
 */
int Vertex::getWeight() {
  return weight;
}

std::string Vertex::getName() {
  return name;
}

std::list<Edge*>::iterator Vertex::getEdgeIterator() {
  return adjacencyList.begin();
}

std::list<Edge*>::iterator Vertex::getEdgeIteratorEnd() {
  return adjacencyList.end();
}

bool Vertex::isVisited() {
  return visited;
}

void Vertex::setVisited() {
  visited = true;
}

void Vertex::unsetVisited() {
  visited = false;
}

void Vertex::deleteEdge(Edge* e) {
  std::list<Edge*>::iterator findIter = std::find(adjacencyList.begin(),
          adjacencyList.end(), e);
  if (findIter != adjacencyList.end()) {
    adjacencyList.erase(findIter);
  }

}
