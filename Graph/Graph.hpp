/* 
 * File:   Graph.hpp
 * Author: Steven Le (cs100fax)
 *
 * Created on December 4, 2012, 2:22 PM
 * 
 * The Graph class can either be directed or undirected. The type of graph 
 * is specified according to its constructor. If a false value is passed in
 * to the constructor, the graph is interpreted as undirected. The default
 * constructor has the graph as directed.
 * 
 * Instance variables are a hash map for its vertices. A hash map is used
 * for quick look up so that creation of new vertices and check for duplicates
 * can be kept to a minimum. In addition, although each vertex has an
 * adjacency for its edges, the graph class has a list for all edges and 
 * this is simply for quick look up of edges rather than having to look up
 * all vertices and then their adjacency list.
 * 
 * Most of its methods comprise of getters except for the methods used
 * to add vertices and edges. The reason for lack of methods to modify
 * the graph for removal of elements is because this assignment did not
 * require such operations. 
 */

#ifndef GRAPH_HPP
#define	GRAPH_HPP
#include <iostream>
#include <map>
#include <string>
#include <list>
#include "Vertex.hpp"

class Graph {
public:

  Graph() {
    directed = true;
  }

  Graph(bool directed) : directed(directed) {
  }

  ~Graph();
  /**
   * Check if vertex is in map container, if not then add it
   * @param Pointer to vertex. Vertex must have its name element initialized
   * 
   */
  void addVertex(Vertex* v);

  /**
   * Takes a pointer to an edge, looks at its values and updates the list
   * of edges for whose vertex it is. In addition, updates own list of edges
   * for quick catalog of all edges rather than having to traverse all
   * vertices to get all the edges.
   * @param e
   */
  void addEdge(Edge* e);

  void addEdge(Edge* e, Edge* eMirror);

  Vertex* getVertex(std::string vertexName);

  bool containsNodeName(std::string name);

  bool isDirected();

  std::map<std::string, Vertex*>::iterator getVertexIterator();
  std::map<std::string, Vertex*>::iterator getVertexIteratorEnd();

  std::list<Edge*>::iterator getEdgeIterator();
  std::list<Edge*>::iterator getEdgeIteratorEnd();

  int getNumOfVertices();
  int getNumOfEdges();

  void unsetAllVisited();
  void deleteEdge(Edge* e);

  static bool graphIsConnected(Graph &g);

  /**
   * Checks to see if graph is connected. If the graph is undirected it simply
   * runs a dfs on one of the vertics. If the graph is directed, it runs a
   * dfs on all of the nodes and sees if everything is seen everytime.
   * 
   * @param g
   * @return 
   */
  static void explore(Vertex* v);

  /**
   * Takes in a graph instance and initializes it. Pseudo factory
   * pattern.
   * @param g Empty new graph.
   * @param inputFile Name of file
   */
  static void createGraph(Graph &g, char * inputFile);
  bool containsEdge(std::string n1, std::string n2);

  void printGraph();
private:
  std::map<std::string, Vertex*> vertices;
  std::list<Edge*> edges;

  std::map<std::pair<std::string, std::string>, Edge*> edgeMap;

  bool directed;
};

#endif	/* GRAPH_HPP */

