#ifndef MATH_HPP
#define MATH_HPP

#include "graph.hpp"

class Node;
class Graph;


struct VectorGeometry {
    VectorGeometry() {
        x = 0;
        y = 0;
    }
    VectorGeometry(Node n1, Node n2) {
        Node diffNode = (n1 - n2);
        x = diffNode.getX();
        y = diffNode.getY();

    }
    int x;
    int y;
    bool isNull() {

        return this->x == 0 && this->y == 0;
    }

};

// all entries are integer so return type can remain integer
int dot(VectorGeometry v1, VectorGeometry v2);
int wedge(VectorGeometry v1, VectorGeometry v2);



#endif