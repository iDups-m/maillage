#include "face.h"
#include <GL/glut.h>
#include <iostream>
#include <string>
// #include "../armadillo/include/armadillo"


//using namespace arma;
using namespace std;



Face::Face()
{
}

Face::Face(string initName) : oneHe(nullptr)
{
    this->name = initName ;
}

Face::Face(HalfEdge * InitHe, string initName)
{
    this->oneHe = InitHe;
    this->name = initName ;

}

void Face::setHalfEdge(HalfEdge* oneHe)
{
    this->oneHe = oneHe ;
}

Face::~Face()
{

}
