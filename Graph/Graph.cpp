/* 
 * File:   Graph.cpp
 * Author: Steven Le (cs100fax)
 * 
 * Created on December 4, 2012, 3:35 PM
 */

#include <cstdlib>
#include <string>
#include "Vertex.hpp"
#include "Graph.hpp"
#include "Edge.hpp"
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
 * Deletes all of the Edges and then deletes all of the Nodes. This order is
 * important because otherwise the delete will attempt to free already 
 * free'd memory
 */
Graph::~Graph() {
  std::list<Edge*>::iterator edgeIter = Graph::getEdgeIterator();

  while (edgeIter != Graph::getEdgeIteratorEnd()) {
    delete (*edgeIter);
    edgeIter++;
  }

  std::map<std::string, Vertex*>::iterator vertexIter =
          Graph::getVertexIterator();

  while (vertexIter != Graph::getVertexIteratorEnd()) {
    delete (*vertexIter).second;
    vertexIter++;
  }


}

/**
 * Each vertex must have a unique name. If the map finds that a vertex with
 * the same name is already in existence, nothing is done. However if it is
 * new i.e not on the hash map, then it is added.
 * @param v vertex. must have unique name not yet used to be added to graph
 */
void Graph::addVertex(Vertex* v) {
  if (!Graph::containsNodeName(v->getName())) {
    vertices[v->getName()] = v;
  }
}

/**
 * Adds new edges to the graph. If the method finds that the vertices for
 * the edges do not exist, it adds the vertices to the graph. After that,
 * it adds the edges to the graph list and updates the adjacency of the 
 * vertices. If the graph is undirected, and the edge for example is
 * (u,v), then (v,u) is also made
 * @param e
 */
void Graph::addEdge(Edge* e) {

  //If edge not yet in the graph, add it, otherwise ignore and return
  if (Graph::containsEdge(e->getStartNode()->getName(),
          e->getEndNode()->getName())) {
    std::cout << "Ignoring duplicate (" << e->getStartNode()->getName() <<
            "," << e->getEndNode()->getName() << ")" << std::endl;

    return;
  }

  //If nodes are not yet in the graph, add them.
  Graph::addVertex(e->getStartNode());
  Graph::addVertex(e->getEndNode());

  //If undirected, make another edge node except reverse start and finish
  if (!directed) {
    Edge* e2 = new Edge(e->getEndNode(), e->getStartNode(),
            e->getMoneyWeight(), e->getTimeWeight());
    edges.push_back(e2);

    edgeMap[std::make_pair(e2->getStartNode()->getName(),
            e2->getEndNode()->getName())] = e2;

    (e2->getStartNode())->addEdge(e2);

    //Update graph list of edges
    edges.push_back(e);
    edgeMap[std::make_pair(e->getStartNode()->getName(),
            e->getEndNode()->getName())] = e;


    //Tell vertex to update edges.
    (e->getStartNode())->addEdge(e);

    //Set mirror edge pointers
    e->setMirrorEdge(e2);
    e2->setMirrorEdge(e);
  } else {

    //Update graph list of edges
    edges.push_back(e);
    edgeMap[std::make_pair(e->getStartNode()->getName(),
            e->getEndNode()->getName())] = e;

    //Tell vertex to update edges.
    (e->getStartNode())->addEdge(e);
  }
}

void Graph::addEdge(Edge* e, Edge* eMirror) {
  if (e->getMirrorEdge() == eMirror) {
    edges.push_back(e);
    edges.push_back(eMirror);

    e->getStartNode()->addEdge(e);
    eMirror->getStartNode()->addEdge(eMirror);
  }
}

/**
 * Returns a pointer to the vertex with the name vertexName
 */
Vertex* Graph::getVertex(std::string vertexName) {
  return vertices[vertexName];
}

/**
 * Checks graph to see if a node by the name of name is in the graph
 * @param name
 * @return true if name already taken, false if not
 */
bool Graph::containsNodeName(std::string name) {
  if (vertices.find(name) == vertices.end()) {
    return false;
  }

  return true;
}

/**
 * Immutable value. Returns true if the graph interprets every edge
 * and vertex as part of a directed or undirected graph
 * @return 
 */
bool Graph::isDirected() {
  return directed;
}

std::map<std::string, Vertex*>::iterator Graph::getVertexIterator() {
  return vertices.begin();
}

std::map<std::string, Vertex*>::iterator Graph::getVertexIteratorEnd() {
  return vertices.end();
}

std::list<Edge*>::iterator Graph::getEdgeIterator() {
  return edges.begin();
}

std::list<Edge*>::iterator Graph::getEdgeIteratorEnd() {
  return edges.end();
}

int Graph::getNumOfVertices() {
  return (int) vertices.size();
}

int Graph::getNumOfEdges() {
  if (Graph::isDirected())
    return (int) edges.size();

  return (int) (edges.size() / 2);
}

void Graph::unsetAllVisited() {
  std::map<std::string, Vertex*>::iterator vertexIter =
          Graph::getVertexIterator();

  while (vertexIter != Graph::getVertexIteratorEnd()) {
    ((*vertexIter).second)->unsetVisited();

    vertexIter++;
  }
}

void Graph::deleteEdge(Edge* e) {
  std::list<Edge*>::iterator findIter = std::find(edges.begin(),
          edges.end(), e);

  if (findIter != edges.end()) {
    Vertex* startNode = e->getStartNode();
    Vertex* endNode = e->getEndNode();

    startNode->deleteEdge(e);
    endNode->deleteEdge(e->getMirrorEdge());

    edges.erase(findIter);
    edges.erase(std::find(edges.begin(), edges.end(), e->getMirrorEdge()));

  }

}

bool Graph::graphIsConnected(Graph &g) {
  if (g.getNumOfVertices() < 2)
    return false;

  g.unsetAllVisited();
  if (!g.isDirected()) {

    Graph::explore((*g.getVertexIterator()).second);

    std::map<std::string, Vertex*>::iterator vertexIter =
            g.getVertexIterator();

    while (vertexIter != g.getVertexIteratorEnd()) {
      //If after explore any node still not visited, then graph is not
      //connected
      if (((*vertexIter).second)->isVisited() == false)
        return false;

      vertexIter++;
    }

    return true;
  }

  return false;
}

void Graph::explore(Vertex* v) {
  v->setVisited();

  std::list<Edge*>::iterator edgeIter = v->getEdgeIterator();
  while (edgeIter != v->getEdgeIteratorEnd()) {
    Vertex* vRef = (*edgeIter)->getEndNode();
    if (!vRef->isVisited())
      Graph::explore(vRef);

    edgeIter++;
  }
}

/**
 * Opens up a text file and expects a certain format. It reads line by line
 * and divides the line into tokens seperated by whitespace. It expects
 * 4 tokens at minimum although anything more than 4 is ignored.
 * The first two tokens are interpreted as an edge (u,v) and vertices
 * u and v are attempted to created followed by an edge instance. The last
 * two tokens are interpreted as the money cost and time cost respectively
 * 
 * The argument it takes in is a graph which it will fill according to the
 * file input
 * @param g graph to be created
 */
void Graph::createGraph(Graph &g, char * inputFile) {

  std::ifstream inFile;
  inFile.open(inputFile);
  if (!inFile.is_open()) {
    std::cout << "Error opening \"" << inputFile << "\"." << std::endl;
    exit(1);
  }

  char c[800];
  while (inFile.good()) {
    inFile.getline(c, 800);
    //Convert input line into string
    std::string str(c);
    std::vector<std::string> tokens;
    //Split string into tokens. Place in tokens container.
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string > (iss),
            std::istream_iterator<std::string > (),
            std::back_inserter<std::vector<std::string> >(tokens));

    if (tokens.size() < 4) {
      throw "Improperly formatted. Not enough arguments";
    }

    std::stringstream ss;
    if (!(ss << tokens [2]) || !(ss << tokens[3])) {
      throw "Improperly formatted. Need 3rd and 4th arg to be int";
    }

    std::string startName = tokens[0];
    std::string endName = tokens[1];
    std::string moneyWeightStr = tokens[2];
    std::string timeWeightStr = tokens[3];

    int moneyWeight = atoi(moneyWeightStr.c_str());
    int timeWeight = atoi(timeWeightStr.c_str());

    Vertex* startNode = new Vertex(startName);
    Vertex* endNode = new Vertex(endName);

    //Check if nodes already exist. If not create them and insert into
    //the graph. If they exist, retrieve and let local point
    if (g.containsNodeName(startName)) {
      delete startNode;
      startNode = g.getVertex(startName);
    } else {
      g.addVertex(startNode);
    }

    if (g.containsNodeName(endName)) {
      delete endNode;
      endNode = g.getVertex(endName);
    } else {
      g.addVertex(endNode);
    }

    Edge* edge = new Edge(startNode, endNode, moneyWeight, timeWeight);

    if (g.containsEdge(startNode->getName(), endNode->getName())) {
      std::cout << "Ignoring duplicate edge (" << startNode->getName() <<
              "," << endNode->getName() << ")" << std::endl;

    } else {
      g.addEdge(edge);
    }

    inFile.peek();
  }

  inFile.close();

}

void Graph::printGraph() {
  std::map<std::string, Vertex*>::iterator vertexIter = Graph::getVertexIterator();

  while (vertexIter != Graph::getVertexIteratorEnd()) {
    std::cout << (*vertexIter).first << std::endl;
    vertexIter++;
  }

  std::cout << "Edges" << std::endl;
  std::list<Edge*>::iterator edgeIter = Graph::getEdgeIterator();

  while (edgeIter != Graph::getEdgeIteratorEnd()) {
    std::cout << "Start: " << (*edgeIter)->getStartNode()->getName() <<
            " End: " << (*edgeIter)->getEndNode()->getName() << std::endl;
    std::cout << "Money: " << (*edgeIter)->getMoneyWeight() << " Time: " <<
            (*edgeIter)->getTimeWeight() << std::endl;
    std::cout << std::endl;

    edgeIter++;
  }
}

bool Graph::containsEdge(std::string n1, std::string n2) {
  if (edgeMap.find(std::make_pair(n1,n2)) == edgeMap.end()) {
    return false;
  }
  return true;
}