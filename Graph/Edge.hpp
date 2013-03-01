/* 
 * File:   Edge.hpp
 * Author: Steven Le (cs100fax)
 *
 * Created on December 4, 2012, 2:23 PM
 * 
 * Header file for the Edge class. The Edge class has 4 instance variables
 * and that is a pointer to its start node, a pointer to its end node, an 
 * integer to hold how much it costs to make, and an integer to hold how long
 * it takes.
 * 
 * It only has 1 constructor and inside that constructor it values to 
 * instantiate all of its instance variables. All of its properties are
 * are immutable and so the only methods it has are getters. In addition
 * there is a struct it uses as a comparator function.
 */

#ifndef EDGE_HPP
#define	EDGE_HPP
#include<iostream>

class Vertex;

class Edge {
public:

  Edge(Vertex* start, Vertex* end, int money, int time) : startNode(start),
  endNode(end), moneyWeight(money), timeWeight(time) {
  }

  Vertex* getStartNode();
  Vertex* getEndNode();
  int getMoneyWeight();
  int getTimeWeight();
  void setMirrorEdge(Edge* e);
  Edge* getMirrorEdge();

  struct PtrComp {

    bool operator()(Edge *one, Edge * other) {
      return one->moneyWeight > other->moneyWeight;
    }
  };

private:
  Vertex* startNode;
  Vertex* endNode;
  int moneyWeight;
  int timeWeight;

  //In an undirected graph(u,v) is the same as (v,u). For our 
  //implementation we create both such edges for undirected. If undirected
  //then we have (u,v) have a pointer to (v,u) and (v,u) have a pointer
  //to (u,v)
  Edge* mirrorEdge;
};

#endif	/* EDGE_HPP */

