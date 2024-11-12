#ifndef INTERSECTGRAPH_HPP
#define INTERSECTGRAPH_HPP

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

struct VectorGeometry {
    VectorGeometry() {
        x = 0.0;
        y = 0.0;
    }
    VectorGeometry(ogdf::GraphAttributes GA, ogdf::node n1, ogdf::node n2) {
          
        x = GA.x(n2) - GA.x(n1);
        y = GA.y(n2) - GA.y(n1);

    }
    VectorGeometry(ogdf::GraphAttributes GA, ogdf::node n, float x, float y) {
        x = x - GA.x(n);
        y = y - GA.y(n);
    }
    VectorGeometry(float x, float y) {
        this->x = x;
        this->y = y;
    } 
    float x;
    float y;
    bool isNull() {

        return this->x == 0.0 && this->y == 0.0;
    }

};

int getSign(int x);
int dot(VectorGeometry v1, VectorGeometry v2);
int wedge(VectorGeometry v1, VectorGeometry v2);
float getNorm(VectorGeometry v);
bool clockwiseOrientation(ogdf::GraphAttributes GA, ogdf::node a, ogdf::node b, ogdf::node c);
bool isSegmentBetween(ogdf::GraphAttributes GA, ogdf::node a,ogdf::node b,ogdf::node c);
bool edgesIntersect(ogdf::GraphAttributes GA, ogdf::edge e1, ogdf::edge e2);
void rearrangeIntGraph(ogdf::GraphAttributes GA);
//void updateOverallIntersectionCount(ogdf::Graph g);




#endif