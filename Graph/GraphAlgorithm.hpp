/* 
 * File:   GraphAlgorithm.hpp
 * Author: stevenle
 *
 * Created on December 6, 2012, 6:44 PM
 */

#ifndef GRAPHALGORITHM_HPP
#define	GRAPHALGORITHM_HPP

class GraphAlgorithm {
public:
  static unsigned long long getTotalMoneyWeight(Graph &g);
  static Graph primsAlgorithm(Graph &g);
  static void dijkstrasAlgorithm(Graph &g, Vertex* v, std::map<Vertex*,
          Vertex*> &prevMap);
  static unsigned long long getTotalTimeWeight(Graph &g);
private:

};

#endif	/* GRAPHALGORITHM_HPP */

