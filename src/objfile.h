#ifndef OBJFILE_H
#define OBJFILE_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

#include "halfEdge.h"
#include "face.h"
#include "vertex.h"

using namespace std;
typedef struct
{
    int vIndex ;
    int heTwinIndex ;
    int faceIndex ;
    int heNextIndex ;
    int hePrevIndex ;
} HeStruct;

typedef struct
{
    double x,y,z ;
    int heIndex ;
} VStruct;

typedef struct
{
    int v0, v1, v2 ;
    int heIndex ;
} FStruct;

class OBJFile
{
public:
    vector<HalfEdge*> tabHalfEdges ;
    vector<Face*> tabFaces;
    vector<Vertex*> tabVertices;

    string fileName ;
    //OBJFile(void);
    OBJFile(string initFileName);
    ~OBJFile(void);
    void readData(void) ;
    void constructTopology(vector<array<double, 3>> vecteurs, vector<array<int, 3>> faces);
    friend ostream& operator<<(ostream& os, const OBJFile& OBJf) ;
};



#endif // OBJFILE_H
