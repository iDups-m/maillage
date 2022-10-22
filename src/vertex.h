#ifndef __VERTEX
#define __VERTEX

#include "halfEdge.h"
#include <string>
//#include <GL/glut.h>

using namespace std;
class HalfEdge ;

class Vertex {
    public :
    double x,y,z;
    HalfEdge* oneHe ;
    string name ;
    int valence;

    struct attr
    {
        int index;
    } attr; 


    Vertex();
    Vertex(double x, double y, double z);
    Vertex(double x, double y, double z, string InitName);
    Vertex(int x, int y, int z, string InitName);
    ~Vertex();

    void setHalfEdge(HalfEdge* oneHe);
    void calculateValence();

};

#endif
