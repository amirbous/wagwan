#ifndef MATHGOEMETRY_HPP
#define MATHGEOMTRY_HPP

#include "../include/graph.hpp"

class Node;
class Edge;
class Graph;


struct VectorGeometry {
    VectorGeometry() {
        x = 0;
        y = 0;
    }
    VectorGeometry(Node& n1, Node& n2) {
          
        x = n2.getX() - n1.getX();
        y = n2.getY() - n1.getY();

    }
    VectorGeometry(int x, int y) {
        this->x = x;
        this->y = y;
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

float getNorm(VectorGeometry v);


#endif