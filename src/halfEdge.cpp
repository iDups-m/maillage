#include "halfEdge.h"
#include <GL/glut.h>
#include <iostream>
#include <fstream>
// #include "../armadillo/include/armadillo"


//using namespace arma;
using namespace std;

//=========================================
HalfEdge::HalfEdge(void)
{


}

HalfEdge::HalfEdge(Vertex* InitVertex, string initName, Face* initFace) : heNext(nullptr), heTwin(nullptr), hePrev(nullptr)
{
    this->vertex = InitVertex ;
    this->name = initName ;
    this->face = initFace ;
}

HalfEdge::HalfEdge(Vertex* InitVertex, string initName)
{
    this->vertex = InitVertex ;
    this->name = initName ;
}



void HalfEdge::setHalfEdges(HalfEdge* twin, HalfEdge* next, HalfEdge* prev)
{
    this->heNext = next ;
    this->heTwin = twin ;
    this->hePrev = prev ;
}



//------------------------------------------
HalfEdge::~HalfEdge(void)
{
}

