#ifndef __FACE
#define __FACE
#include "halfEdge.h"
#include <string>

//#include <GL/glut.h>
using namespace std;
class HalfEdge ;

class Face 
{
    public:
    HalfEdge* oneHe ;
    string name ;
    //FaceAttribut attributs ;
    Face() ;
    Face(string initName) ;
    Face(HalfEdge * InitHe, string initName) ;
    ~Face() ;

    void setHalfEdge(HalfEdge* oneHe);
};



#endif
