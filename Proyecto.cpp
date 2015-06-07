//This example program is created by thecplusplusuy for demonstration purposes. It's a simple Wavefront (.obj) loader:
//http://www.youtube.com/user/thecplusplusguy
//Free source, modify if you want, LGPL licence (I guess), I would be happy, if you would not delete the link
//so other people can see the tutorial
//compile under Linux:
//g++ objloader.cpp -lSDL -lGL -lGLU
//under Windows, set the libraries as I showed you in the tutorial 0
#include <SDL/SDL.h> //check your include folder, it may be just SDL.h

#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <iostream>
 
GLint t=5;
GLint ancho=400;
GLint alto=400;
float A,D,W =10;
float S=10;
int Q=0;
bool X,Z=true;
int x,y=0;
int z=5, shading=0, posluz1=0; 
int rotacion=0;
int incrementoPendulo=1;
int anguloPendulo=0;
float pos[]={1.0,5.0,9.0,1.0};        //set the position
        
 
struct coordinate{
        float x,y,z;
        coordinate(float a,float b,float c) : x(a),y(b),z(c) {};
};
 
//for faces, it can contain triangles and quads as well, the four variable contain which is that
struct face{
        int facenum;
        bool four;
        int faces[4];
        face(int facen,int f1,int f2,int f3) : facenum(facen){  //constructor for triangle
                faces[0]=f1;
                faces[1]=f2;
                faces[2]=f3;
                four=false;
        }
        face(int facen,int f1,int f2,int f3,int f4) : facenum(facen){ //overloaded constructor for quad
                faces[0]=f1;
                faces[1]=f2;
                faces[2]=f3;
                faces[3]=f4;
                four=true;
        }
};
 
float angle=0.0;        //we rotate or object with angle degrees
 float angle2=0.0;
int loadObject(const char* filename)
{
        std::vector<std::string*> coord;        //read every single line of the obj file as a string
        std::vector<coordinate*> vertex;
        std::vector<face*> faces;
        std::vector<coordinate*> normals;       //normal vectors for every face
        std::ifstream in(filename);     //open the .obj file
        if(!in.is_open())       //if not opened, exit with -1
        {
                std::cout << "Nor oepened" << std::endl;
                return -1;
        }
        char buf[256];
        //read in every line to coord
        while(!in.eof())
        {
                in.getline(buf,256);
                coord.push_back(new std::string(buf));
        }
        //go through all of the elements of coord, and decide what kind of element is that
        for(int i=0;i<coord.size();i++)
        {
                if(coord[i]->c_str()[0]=='#')   //if it is a comment (the first character is #)
                        continue;       //we don't care about that
                else if(coord[i]->c_str()[0]=='v' && coord[i]->c_str()[1]==' ') //if vector
                {
                        float tmpx,tmpy,tmpz;
                        sscanf(coord[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);       //read in the 3 float coordinate to tmpx,tmpy,tmpz
                        vertex.push_back(new coordinate(tmpx,tmpy,tmpz));       //and then add it to the end of our vertex list
                }else if(coord[i]->c_str()[0]=='v' && coord[i]->c_str()[1]=='n')        //if normal vector
                {
                        float tmpx,tmpy,tmpz;   //do the same thing
                        sscanf(coord[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
                        normals.push_back(new coordinate(tmpx,tmpy,tmpz));     
                }else if(coord[i]->c_str()[0]=='f')     //if face
                {
                        int a,b,c,d,e;
                        if(count(coord[i]->begin(),coord[i]->end(),' ')==3)     //if it is a triangle (it has 3 space in it)
                        {
                  sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b);
                                faces.push_back(new face(b,a,c,d));     //read in, and add to the end of the face list
                        }else{
                                sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&a,&b,&c,&b,&d,&b,&e,&b);
                                faces.push_back(new face(b,a,c,d,e));   //do the same, except we call another constructor, and we use different pattern
                        }
                }
        }
//raw
        int num;        //the id for the list
        num=glGenLists(1);      //generate a uniqe
        glNewList(num,GL_COMPILE);      //and create it
        for(int i=0;i<faces.size();i++)
        {
                if(faces[i]->four)      //if it's a quad draw a quad
                {
                        glBegin(GL_QUADS);
                                //basically all I do here, is use the facenum (so the number of the face) as an index for the normal, so the 1st normal owe to the first face
                                //I subtract 1 because the index start from 0 in C++
                                glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);
                                //draw the faces
                                glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
                                glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
                                glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
                                glVertex3f(vertex[faces[i]->faces[3]-1]->x,vertex[faces[i]->faces[3]-1]->y,vertex[faces[i]->faces[3]-1]->z);
                        glEnd();
                }else{
                        glBegin(GL_TRIANGLES);
                                glNormal3f(normals[faces[i]->facenum-1]->x,normals[faces[i]->facenum-1]->y,normals[faces[i]->facenum-1]->z);
                                glVertex3f(vertex[faces[i]->faces[0]-1]->x,vertex[faces[i]->faces[0]-1]->y,vertex[faces[i]->faces[0]-1]->z);
                                glVertex3f(vertex[faces[i]->faces[1]-1]->x,vertex[faces[i]->faces[1]-1]->y,vertex[faces[i]->faces[1]-1]->z);
                                glVertex3f(vertex[faces[i]->faces[2]-1]->x,vertex[faces[i]->faces[2]-1]->y,vertex[faces[i]->faces[2]-1]->z);
                        glEnd();
                }
        }
        glEndList();
        //delete everything to avoid memory leaks
        for(int i=0;i<coord.size();i++)
                delete coord[i];
        for(int i=0;i<faces.size();i++)
                delete faces[i];
        for(int i=0;i<normals.size();i++)
                delete normals[i];
        for(int i=0;i<vertex.size();i++)
                delete vertex[i];
        return num;     //return with the id
}
 
int cube;
int cube2;
void init()
{
    glClearColor(1,1,1,1);
    
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,640.0/480.0,1.0,500.0);
	//gluLookAt(A,4,S, 0, 0, 0, 0,5,0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

	cube=loadObject("pendulo.obj");
	cube2=loadObject("engrane.obj");
	//cube3=loadObject("engrane.obj");
	//cube4=loadObject("p.obj");
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
    GLfloat luz_ambiental[] = { 0.9, 0.2, 0.2, 0.0 };
    GLfloat light_position[] = { posluz1, 5,5,1}; //posicion de la luz X, Y , Z y direccional o no
    GLfloat light_spot1[]= {0.0, -5.0, -15}; //donde termina la luz
    GLfloat light_exponent[]={0.0};
    GLfloat light_cut[]={120.0};   //Abertura en grados del foco
    GLfloat light_atenuacion[]={0.6}; //coeficiente de atenuacion
    ancho = 600;
    alto = 600;

    glEnable(GL_LIGHTING);   //activacion de luces principales
    glEnable(GL_LIGHT0);    //habilitacion de la luz 0
    
    glDepthFunc(GL_LESS);   //activacion de 3D
    glEnable(GL_DEPTH_TEST);
   
    //implementacion de parametros LIGHT
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);             //posicion de la luz
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_spot1);          //direccion
    glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT,light_exponent);
    glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF,light_cut);                //angulo de foco
    glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION,light_atenuacion);  //atenuacion constante no cambia con lo lejos de la luz
    //glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION,light_atenuacion);    //atenuacion  linear entre mas lejos mas cambia
    //glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION,light_atenuacion); //atenuacion cuadratica entre mas lejos y mas abierto este

    
    
    //Parametros de las luces en material
    
    GLfloat Ambiente[4] = {0.1f,0.1f,0.1f,1.0f};            // luz ambiental
    GLfloat Difusa[4] = {0.0f,0.0f,0.0f,0.0f};              //reflexion difusa
    GLfloat Specular[4] = {0.7f,0.7f,0.7f,1.0f};            //Color de brillo
    
    GLfloat Brillo = 100.0f;                           
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, Ambiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, Difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
    glMaterialf(GL_FRONT, GL_SHININESS, Brillo);
    
    Ambiente[0] = 1;Ambiente[1]=1; Ambiente[2]=1;Ambiente[3]=0;            
   
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,Ambiente);
    glPolygonMode(GL_FRONT, GL_FILL);
    glColorMaterial (GL_FRONT, GL_AMBIENT);	
    glEnable(GL_COLOR_MATERIAL); 
    
    
    glEnable(GL_NORMALIZE); 

	glEnable(GL_LIGHTING);    
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH); 
		
	
}

void display()
{
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        //Pendulo
        glPushMatrix();
        glLightfv(GL_LIGHT0,GL_POSITION,pos);

        glTranslatef(0.0,0.0,-18.0);   
        glRotatef(anguloPendulo, 0,0,1);
        
        if(anguloPendulo==20)
			incrementoPendulo*=-1;
		else if(anguloPendulo==-20)
			incrementoPendulo*=-1;
		anguloPendulo+=incrementoPendulo;
		
        
        glScalef(0.5,0.5,0.5);
       
        glCallList(cube);       //draw the 3D mesh
        glPopMatrix();
        
        
        //Engrane
        glPushMatrix();

        glTranslatef(-0.2,2.1,-18.0);   
        
        glRotatef(rotacion,0,0,1);
        if(rotacion>360)
			rotacion%=360;
        rotacion++;
        glScalef(0.5,0.5,0.5);

        glColor3f(1.0,0.0,0.0);
        glCallList(cube2); 

        glPopMatrix();
        
        
		glLightfv(GL_LIGHT0,GL_POSITION,pos);
		
}
 
 
int main(int argc,char** argv)
{
        SDL_Init(SDL_INIT_EVERYTHING);
        SDL_Surface* screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE|SDL_OPENGL);
        bool running=true;
        Uint32 start;
        SDL_Event event;
        init();
        while(running)
        {
                start=SDL_GetTicks();
                while(SDL_PollEvent(&event))
                {
                        switch(event.type)
                        {
                                case SDL_QUIT:
                                        running=false;
                                        break;
                        }
                }
                display();
                SDL_GL_SwapBuffers();
                angle2+=1.0;
                
                if(angle>360)
                        angle-=360;
                if(1000/30>(SDL_GetTicks()-start))
                        SDL_Delay(1000/30-(SDL_GetTicks()-start));
        }
        SDL_Quit();
        return 0;      
}

