#include "../include/mathGeometry.hpp"

class Node;
class Graph;

int dot(VectorGeometry v1, VectorGeometry v2){

    return v1.x*v2.x + v1.y*v2.y;
}
int wedge(VectorGeometry v1, VectorGeometry v2){
    return v1.x*v2.y - v1.y*v2.x;

}