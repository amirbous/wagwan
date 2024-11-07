#include "../include/mathGeometry.hpp"
#include "../include/graph.hpp"
#include <cmath>


int dot(VectorGeometry v1, VectorGeometry v2){

    return v1.x*v2.x + v1.y*v2.y;
}
int wedge(VectorGeometry v1, VectorGeometry v2){
    return v1.x*v2.y - v1.y*v2.x;

}
float getNorm(VectorGeometry v) {
    return sqrt(v.x * v.x + v.y * v.y);
}