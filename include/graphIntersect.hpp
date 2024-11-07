#ifndef GRAPHINTERSECT_HPP
#define GRAPHINTERSECT_HPP

#include <iostream>
#include "../include/graph.hpp"
#include "../include/algorithms/lineSweeper.hpp"



bool clockwiseOrientation(Node a, Node b, Node c);
bool isSegmentBetween(Node a, Node b, Node c);
bool edgesIntersect(Graph g, Edge e1, Edge e2);
void updateOverallIntersectionCount(Graph& g);

#endif