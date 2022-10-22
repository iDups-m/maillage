#include "objfile.h"
#include <string>
#include <sstream>
#include <array>
#include <algorithm>

using namespace std;
//-------------------------------------------
OBJFile::OBJFile(string initFileName)
{
  this->fileName = initFileName ;
  //cout << "dans init OBJFile "<<endl;
}
//-------------------------------------------
// OBJFile::OBJFile(void)
// {

// }
//-------------------------------------------
OBJFile::~OBJFile(void)
{


}
//-------------------------------------------
ostream& operator<<(ostream& os, const OBJFile& OBJf)
{

 }
//-------------------------------------------

void OBJFile::readData(void)
{
  string line;
  int nbVertices(0), nbFaces(0);

  ifstream myfile(this->fileName);

  if (myfile.is_open()) {

    vector<array<double, 3>> vecteurs; //VBO
    vector<array<int, 3>> faces; //IBO

    while( getline(myfile,line) ){
      float x,y,z ;
      char carLine;

      //cout << line << '\n';
      istringstream istr(line);
      istr >> carLine >> x >> y >> z ;

      //**********************************************************
      // AFAIRE
      // ici stoker ces info dans une structure de données
      //**********************************************************

      if(carLine == 'v'){
        array<double, 3> tabTmpV = {x, y, z};
        vecteurs.push_back(tabTmpV);
        nbVertices++;
      }

      if(carLine == 'f'){
        array<int, 3> tabTmpF = {(int)x, (int)y, (int)z};
        faces.push_back(tabTmpF);
        nbFaces++;
      }

    }

    myfile.close();

    this->constructTopology(vecteurs, faces);
  }

  else cout << "Unable to open file";

}
//-----------------------------------------------------
void OBJFile::constructTopology(vector<array<double, 3>> vecteurs, vector<array<int, 3>> faces)
{
  //**********************************************************
  // AFAIRE
  // ici exploiter les données du fichier OBJ pour construire les tableaux d'indices
  //**********************************************************

  int i=0;

  // sommets
  for(auto vec : vecteurs){
    Vertex *v = new Vertex(vec[0], vec[1], vec[2], "v" + to_string(i+1));
    v->attr.index = i++;
    tabVertices.push_back(v);
  }

  // faces
  i = 0;
  int iHe = 0;
  for(auto face : faces){
    Face *f = new Face("f" + to_string(i));

    // pour chaque couple de vertex (-> arrete)
    for(int j=0; j<3; ++j){
      int vFirst = face[j % 3];
      int vSecond = face[(j+1) % 3];

      Vertex *v = tabVertices[vFirst -1];
      HalfEdge *he = new HalfEdge(v, "he" + to_string(iHe), f);
      tabHalfEdges.push_back(he);

      if(v->oneHe == nullptr){
        v->setHalfEdge(he);
      }

      ++iHe;
    }


    // chainage next prev
    int nbHe = tabHalfEdges.size();
    HalfEdge *he1 = tabHalfEdges[nbHe-3];
    HalfEdge *he2 = tabHalfEdges[nbHe-2];
    HalfEdge *he3 = tabHalfEdges[nbHe-1];

    he1->heNext = he2;
    he1->hePrev = he3;

    he2->heNext = he3;
    he2->hePrev = he1;

    he3->heNext = he1;
    he3->hePrev = he2;

    // mémoriser sommets de fin pour chaque he pour le twin plus tard
    he1->attr.vertexEnd = he2->vertex;
    he2->attr.vertexEnd = he3->vertex;
    he3->attr.vertexEnd = he1->vertex;

    //lien entre Face et He
    f->oneHe = he1;

    ++i;
    tabFaces.push_back(f);
  }


  // pour indiquer les twin de chaque he
  vector<HalfEdge*> TMPtabHalfEdges = tabHalfEdges;
  vector<HalfEdge*> tabHeBords;

  for(int k=0; k<tabHalfEdges.size(); ++k){
    HalfEdge *he = tabHalfEdges[k];
    bool founded = false;

    for(int l=0; l<TMPtabHalfEdges.size(); ++l){
      HalfEdge *heTemp = TMPtabHalfEdges[l];

      if( (he->vertex == heTemp->attr.vertexEnd) && (heTemp->vertex == he->attr.vertexEnd) ){
        he->heTwin = heTemp;

        /*
        heTemp->heTwin = he;

        // diminution de la taille de la liste pour accélérer la recherche (car plus he et heTemp traités)
        TMPtabHalfEdges.erase(TMPtabHalfEdges.begin() + l);
        auto it = find(TMPtabHalfEdges.begin(), TMPtabHalfEdges.end(), he);
        if (it != TMPtabHalfEdges.end()){
          TMPtabHalfEdges.erase(it);
        }
        */

        founded = true;
        break;
      }
    }

    if(!founded){
      // arrete du bord qui n’a pas pu être créée car pas de face
      HalfEdge *heBord = new HalfEdge(he->heNext->vertex, "he" + to_string(iHe));
      he->heTwin = heBord;
      heBord->heTwin = he;
      tabHeBords.push_back(heBord);
    }

  }


  // dernière étape : next et prev des heBords
  for (auto heBord : tabHeBords){
    tabHalfEdges.push_back(heBord);
    
    heBord->heNext = heBord->heTwin->hePrev->heTwin->hePrev->heTwin;
    heBord->hePrev = heBord->heTwin->heNext->heTwin->heNext->heTwin;
  }


}


