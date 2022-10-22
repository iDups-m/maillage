#include "vertex.h"
#include <GL/glut.h>
#include <iostream>
// #include "../armadillo/include/armadillo"


//using namespace arma;
using namespace std;

//=========================================	
Vertex::Vertex()
{
	this->valence = 0;
}
Vertex::Vertex(double x, double y, double z) : name(""), oneHe(nullptr)
{
    this->x = x ;
    this->y = y ;
    this->z = z ;
    this->valence = 0;
}

Vertex::Vertex(double x, double y, double z, string InitName) : oneHe(nullptr)
{
    this->x = x ;
    this->y = y ;
    this->z = z ;
    this->name = InitName ;
    this->valence = 0;
}

Vertex::Vertex(int x, int y, int z, string InitName) : oneHe(nullptr)
{
    this->x = (double) x ;
    this->y = (double) y ;
    this->z = (double) z ;
    this->name = InitName ;
    this->valence = 0;
}

void Vertex::setHalfEdge(HalfEdge* oneHe)
{
    this->oneHe = oneHe ;
    this->valence = 0;
}

Vertex::~Vertex()
{

}


void Vertex::calculateValence(){
    int valence = 0;
    HalfEdge* start_edge = this->oneHe;
    HalfEdge* current_edge = start_edge;
    do {
        valence++;
        current_edge = current_edge->hePrev->heTwin;
    } while (current_edge != start_edge);
    this->valence = valence;
}
