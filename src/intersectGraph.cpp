
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include "../include/intersectGraph.hpp"


int getSign(float x){
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
};
int dot(VectorGeometry v1, VectorGeometry v2){

    return v1.x*v2.x + v1.y*v2.y;
}
int wedge(VectorGeometry v1, VectorGeometry v2){
    return v1.x*v2.y - v1.y*v2.x;

}
float getNorm(VectorGeometry v) {
    return sqrt(dot(v, v));
}

int orientation(const ogdf::GraphAttributes GA, ogdf::node a, ogdf::node b, ogdf::node c) {
    int val = (GA.y(c) - GA.y(a)) * (GA.x(b) - GA.x(a)) -
              (GA.y(b) - GA.y(a)) * (GA.x(c) - GA.x(a));

    if (val == 0) return 0;
    return (val > 0) ? 1 : 2; 
}

bool onSegment(const ogdf::GraphAttributes GA, ogdf::node a, ogdf::node b, ogdf::node c) {
    return std::min(GA.x(a), GA.x(b)) <= GA.x(c) && GA.x(c) <= std::max(GA.x(a), GA.x(b)) &&
           std::min(GA.y(a), GA.y(b)) <= GA.y(c) && GA.y(c) <= std::max(GA.y(a), GA.y(b));
}

bool edgesIntersect(ogdf::GraphAttributes GA, ogdf::edge e1, ogdf::edge e2) {
    ogdf::node a = e1->source();
    ogdf::node b = e1->target();
    ogdf::node c = e2->source();
    ogdf::node d = e2->target();

    // Big missing section ==> if shared node
    if (a == c || a == d || b == c || b == d) {
        return false; 
    }


    int o1 = orientation(GA, a, b, c);
    int o2 = orientation(GA, a, b, d);
    int o3 = orientation(GA, c, d, a);
    int o4 = orientation(GA, c, d, b);

  
    if (o1 != o2 && o3 != o4) return true;


    if (o1 == 0 && onSegment(GA, a, b, c)) return true;
    if (o2 == 0 && onSegment(GA, a, b, d)) return true;
    if (o3 == 0 && onSegment(GA, c, d, a)) return true;
    if (o4 == 0 && onSegment(GA, c, d, b)) return true;

    return false;
}