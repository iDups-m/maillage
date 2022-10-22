#include "mesh.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>
// #include "../armadillo/include/armadillo"


//using namespace arma;
using namespace std;

	
//=========================================	
Mesh::Mesh()	
{
}

Mesh::~Mesh()
{
}


void Mesh::setVertices(vector <Vertex*> _vertices){
    vertices = _vertices;
}

void Mesh::setHedges(vector <HalfEdge*> _hedges){
    hedges = _hedges;
}

void Mesh::setFaces(vector <Face*> _faces){
    faces = _faces;
}

void Mesh::getValence(){
    for(int i = 0; i < vertices.size(); i++){
        vertices[i]->calculateValence();
        cout << "Vertex " << vertices[i]->name << " has valence " << vertices[i]->valence << endl;
    }
}
