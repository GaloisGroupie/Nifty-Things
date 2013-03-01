# A simple makefile for CSE 100 P3

CC=g++
CXXFLAGS=-g
LDFLAGS=-g

all: netplan

netplan: Vertex.o Graph.o Edge.o GraphAlgorithm.o netplan.o 
	g++ -o netplan netplan.o Vertex.o Graph.o Edge.o GraphAlgorithm.o
	
netplan.o: Vertex.hpp Graph.hpp Edge.hpp GraphAlgorithm.hpp
	g++ -c netplan.cpp Vertex.cpp Graph.cpp Edge.cpp GraphAlgorithm.cpp
	
GraphAlgorithm.o: Vertex.hpp Graph.hpp Edge.hpp GraphAlgorithm.hpp
	g++ -c Vertex.cpp Graph.cpp Edge.cpp GraphAlgorithm.cpp
	
Vertex.o: Vertex.hpp Edge.hpp
	g++ -c Vertex.cpp Edge.cpp

Edge.o: Edge.hpp
	g++ -c Edge.cpp

Graph.o: Vertex.hpp Graph.hpp Edge.hpp
	g++ -c Vertex.cpp Graph.cpp Edge.cpp

clean:
	rm -f netplan *.o core*

purify:
	purify -cache-dir=$HOME g++ netplan.cpp Vertex.cpp Graph.cpp Edge.cpp GraphAlgorithm.cpp -o netplan
