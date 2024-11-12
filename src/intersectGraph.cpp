
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

bool clockwiseOrientation(ogdf::GraphAttributes GA, ogdf::node a, ogdf::node b, ogdf::node c) {

    return (GA.y(c)-GA.y(a))*(GA.x(b)-GA.x(a)) > (GA.y(b)-GA.y(a))*(GA.x(c)-GA.x(a));

}
bool isSegmentBetween(ogdf::GraphAttributes GA, ogdf::node a,ogdf::node b,ogdf::node c) {
    VectorGeometry v1 = VectorGeometry(GA, a, b);
    VectorGeometry v2 = VectorGeometry(GA, b, c);

    return wedge(v1, v2) == 0 && dot(v1, v2) > 0;
}
bool edgesIntersect(ogdf::GraphAttributes GA, ogdf::edge e1, ogdf::edge e2) {
    bool nonParallelIntersect = false;
    ogdf::node a = e1->source();
    ogdf::node b = e1->target();
        
    ogdf::node c = e2->source();
    ogdf::node d = e2->target();

        
    nonParallelIntersect = clockwiseOrientation(GA, a,c,d) != clockwiseOrientation(GA, b,c,d) && clockwiseOrientation(GA, a,b,c) != clockwiseOrientation(GA, a,b,d);


    return nonParallelIntersect;// general case intersection 
        //|| isSegmentBetween(GA, a, b, d) || isSegmentBetween(GA, c, d, a); // Overlapping parallel segments
}

void rearrangeIntGraph(ogdf::GraphAttributes GA) {
    return;
}