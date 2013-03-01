/* 
 * File:   Edge.cpp
 * Author: Steven Le (cs100fax)
 * 
 * Created on December 4, 2012, 3:20 PM
 */

#include "Edge.hpp"
#include "Vertex.hpp"

Vertex* Edge::getStartNode() {
  return startNode;
}

Vertex* Edge::getEndNode() {
  return endNode;
}

int Edge::getMoneyWeight() {
  return moneyWeight;
}

int Edge::getTimeWeight() {
  return timeWeight;
}

void Edge::setMirrorEdge(Edge* e) {
  mirrorEdge = e;
}

Edge* Edge::getMirrorEdge() {
  return mirrorEdge;
}

