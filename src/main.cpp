
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include "halfEdge.h"
#include "vertex.h"
#include "face.h"
#include "mesh.h"
#include <vector>
#include <sstream>
#include"objfile.h"
#include<string>

using namespace std ;
void affichage(void);

void clavier(unsigned char touche,int x,int y);
void affiche_repere(void);

void mouse(int, int, int, int);
void mouseMotion(int, int);
//void reshape(int,int);


// variables globales pour OpenGL
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance=0.;


#define NBFACES 6
#define NBVERTICES 7
#define NBHALFEDGES 24
Mesh* ExMesh;
OBJFile *objFile;
string fileName;

void initMeshWithOBJFile(void){
  objFile = new OBJFile(fileName);
  
  objFile->readData();

  ExMesh = new Mesh();
  ExMesh->setVertices(objFile->tabVertices);
  ExMesh->setHedges(objFile->tabHalfEdges);
  ExMesh->setFaces(objFile->tabFaces);
}

//----------------------------------------------------------------------------------
void initMesh()
//----------------------------------------------------------------------------------
{
    int tabHe[NBHALFEDGES][5]={  // sommet, heTxin, face, heNext, hePrev pour chaque demi-arete
        { 0,18, 0, 1, 2},
        { 2,11, 0, 2, 0},
        { 3,3 , 0, 0, 1},
        { 0, 2, 1, 4, 5},
        { 3, 6, 1, 5, 3},
        { 1,19, 1, 3, 4},
        { 1, 4, 2, 7, 8},
        { 3,17, 2, 8, 6},
        { 4,20, 2, 6, 7},
        { 2,21, 3,10,11},
        { 5,12, 3,11, 9},
        { 3, 1, 3, 9,10},
        { 3,10, 4,13,14},
        { 5,22, 4,14,12},
        { 6,15, 4,12,13},
        { 3,14, 5,16,17},
        { 6,23, 5,17,15},
        { 4, 7, 5,15,16},
        { 2, 0,-1,19,21},
        { 0, 5,-1,20,18},
        { 1, 8,-1,23,19},
        { 5, 9,-1,18,22},
        { 6,13,-1,21,23},
        { 4,16,-1,22,20}   };

    int tabFace[NBFACES] ={0,3,6,9,12,15}; // he pour chaque face

    int tabVertex[NBVERTICES][4] ={          // x,y,z, he pour chaque sommet
                            {1,4,0,0},
                            {3,4,0,5},
                            {0,2,0,1},
                            {2,2,0,2},
                            {4,2,0,8},
                            {1,0,0,10},
                            {3,0,0,14}};

   ExMesh = new Mesh() ;
    //***********************************************
    // TODO
    // ici construire le maillage haldEdge "ExMesh" à partir des tableaux d'incides tabHe, tabFace, tabVertex

    vector <Vertex*> vertexes;
    vector <Face*> faces;
    vector <HalfEdge*> edges ;
    string baseName;

    /* Ajout des sommets, halfedge pointant vers null */
    for(int i=0; i<NBVERTICES; ++i){
      baseName = "v";
      vertexes.push_back(new Vertex(tabVertex[i][0], tabVertex[i][1], tabVertex[i][2], baseName.append(to_string(i+1))));
    }

    /* Ajout des faces, halfedge pointant vers null */
    for(int i=0; i<NBFACES; ++i) {
      baseName = "f";
      faces.push_back(new Face(baseName.append(to_string(i))));
    }

    /* Ajout des demi-arretes, next twin et prev pointant vers null */
    for(int i=0; i<NBHALFEDGES; ++i) {
      baseName = "e";
      Vertex *v = vertexes[tabHe[i][0]];
      Face *f = faces[tabHe[i][2]];
      if(tabHe[i][2] == -1) f = nullptr;
      edges.push_back(new HalfEdge(v, baseName.append(to_string(i)), f));
    }


    /* Ajout de l'halfhedge dans les sommets */
    for(int i=0; i<NBVERTICES; ++i){
      vertexes[i]->setHalfEdge(edges[tabVertex[i][3]]);
    }

    /* Ajout de l'halfhedge dans les faces */
    for(int i=0; i<NBFACES; ++i){
      faces[i]->setHalfEdge(edges[tabFace[i]]);
    }

    /* Ajout des halfhedges next twin et prev dans les demi-arretes */
    for(int i=0; i<NBHALFEDGES; ++i){
      edges[i]->setHalfEdges(edges[tabHe[i][1]], edges[tabHe[i][3]], edges[tabHe[i][4]]);
    }



    ExMesh->setVertices(vertexes);
    ExMesh->setHedges(edges);
    ExMesh->setFaces(faces);


    // Show valence of vertices of the mesh
    //ExMesh->getValence();

    //***********************************************
}



//----------------------------------------------------------------------------------
void initOpenGl()
//----------------------------------------------------------------------------------
{
//initMesh();
initMeshWithOBJFile();

//lumiere 

	glClearColor( .5, .5, 0.5, 0.0 );
 
	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat l_pos[] = { 3.,3.5,3.0,1.0 };
    glLightfv(GL_LIGHT0,GL_POSITION,l_pos);

     glLightfv(GL_LIGHT0,GL_DIFFUSE,l_pos);
     glLightfv(GL_LIGHT0,GL_SPECULAR,l_pos);
 
     // glDepthFunc(GL_LESS);
     // glEnable(GL_DEPTH_TEST);
     glEnable(GL_COLOR_MATERIAL);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(45.0f,(GLfloat)200/(GLfloat)200,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();	
//	glScalef(.7,.7,.7);
    gluLookAt(0.,0.,4., 0.,0.,0., 0.,1.,0.);
  //  glTranslatef(0.0,0.0,-5.0);
}

//------------------------------------------------------
void displayHalfEdge(void)
//----------------------------------------------------------------------------------
{
    //**********************************************************************
    // TODO
    // Écrire la visualisation du maillage "ExMesh

    glColor3f(0.0,0.0,1.0);
    glBegin(GL_TRIANGLES);
      for(auto f: ExMesh->faces) {
        HalfEdge *he = f->oneHe;
        do {
          glVertex3d(he->vertex->x, he->vertex->y, he->vertex->z);
          he = he->heNext;
        } while(he != f->oneHe);
      }
    glEnd();

    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
      for(auto he: ExMesh->hedges) {
        if(he->face == nullptr) {
          Vertex *v = he->vertex;
          glVertex3d(v->x, v->y, v->z);
          v = he->heTwin->vertex;
          glVertex3d(v->x, v->y, v->z);
        }
      }
    glEnd();

    //**********************************************************************

}
int main(int argc,char **argv)
{


  if(argc == 2){
    fileName = argv[1];
    cout << "Constuction du maillage à partir du fichier " << fileName << endl;
  } else {
    fileName = "test.obj";
    cout << "No file name given, using default file name: " << fileName << endl;
  }

  /* initialisation de glut et creation
     de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(600,600);
  glutCreateWindow("ifs");

  /* Initialisation d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(1.0);
	
  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotion);

initOpenGl() ;

/* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}
//------------------------------------------------------
void affiche_repere(void)
{
  glBegin(GL_LINES);
  glColor3f(1.0,0.0,0.0);
  glVertex2f(0.,0.);
  glVertex2f(1.,0.);
  glEnd(); 

	 glBegin(GL_LINES);
  glColor3f(0.0,1.0,0.0);
  glVertex2f(0.,0.);
  glVertex2f(0.,1.);
  glEnd(); 
   glBegin(GL_LINES);
  glColor3f(0.0,0.0,1.0);
  glVertex3f(0.,0.,0.);
  glVertex3f(0.,0.,1.);
  glEnd(); 
}

//-----------------------------------------------------



//------------------------------------------------------
void affichage(void)
{
	glMatrixMode(GL_MODELVIEW);   
  /* effacement de l'image avec la couleur de fond */
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0,0,cameraDistance);
    glRotatef(cameraAngleX,1.,0.,0.)	;
    glRotatef(cameraAngleY,0.,1.,0.);
    //--------------------------------
    affiche_repere();
    //--------------------------------
    displayHalfEdge();
    //--------------------------------

    glPopMatrix();
  /* on force l'affichage du resultat */
  glFlush();
}

//------------------------------------------------------


//------------------------------------------------------
void clavier(unsigned char touche,int x,int y)
{

  switch (touche)
    {
    case '+':
      glutPostRedisplay();
      break;
    case '-':
      glutPostRedisplay();
      break;
    case 'f': //* affichage en mode fil de fer 
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
      case 'p': //* affichage du carre plein 
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
  case 's' : //* Affichage en mode sommets seuls 
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'q' : //*la touche 'q' permet de quitter le programme 
      exit(0);
    }
    
}
void mouse(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }

    else if(button == GLUT_MIDDLE_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseMiddleDown = true;
        }
        else if(state == GLUT_UP)
            mouseMiddleDown = false;
    }
}


void mouseMotion(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if(mouseRightDown)
    {
        cameraDistance += (y - mouseY) * 0.2f;
        mouseY = y;
    }

    glutPostRedisplay();
}

    
    
