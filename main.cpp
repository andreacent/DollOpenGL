#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>

using namespace std;

#define DEF_floorGridScale  1.0f
#define DEF_floorGridXSteps 12.0f
#define DEF_floorGridZSteps 12.0f

//INICIALIZO LOS ANGULOS
float   rotT=0.0,rotC=0.0,   //Torso, cabeza
        rotBi=0.0,rotBd=0.0, //hombro
        rotMi=0.0,rotMd=0.0, //muneca
        rotCi=0.0,rotCd=0.0, //codo
        rotPi=0.0,rotPd=0.0, //pierna
        rotRi=0.0,rotRd=0.0, //rodilla
        rotTi=0.0,rotTd=0.0, //tobillo
        ang = 30.0;          //angulo

char    area;

//COLORES
GLfloat const red[3]     = {1,0,0}, 
              blue[3]    = {0,0,1}, 
              cyan[3]    = {0,1,1}, 
              white[3]   = {1,1,1}, 
              green[3]   = {0,1,0}, 
              yellow[3]  = {1,1,0}, 
              magenta[3] = {1,0,1}; 


void changeViewport(int w, int h) {
    float aspectratio;
    aspectratio = (float) w / (float) h ;

    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (w <= h){
        glOrtho(-12.0, 12.0, -12.0/aspectratio, 12.0/aspectratio, -1.0, 1.0); 
    }else{
        glOrtho(-12.0*aspectratio, 12.0*aspectratio, -12.0, 12.0, -1.0, 1.0);
    }
}

//  DIBUJA CIRCUNFERENCIA
void drawCircle(float px, float py, float radio) {
    float calx,caly;
    glPointSize(1.0);
    glBegin(GL_POINTS);
        for(double i=0.0; i<10; i+=0.001){
            calx=radio*cos(i)+px;
            caly=radio*sin(i)+py;
            glVertex2f(calx,caly);
        }
    glEnd();
}

//  DIBUJA UN PUNTO (ARTICULACION)
void drawPoint(float x, float y){
    glColor3fv(red);
    glPointSize(8.0);
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

// DIBUJA RECTAS
void drawLine(float x, float y){
    glBegin(GL_LINES);
        glVertex2f(0.0, 0.0);
        glVertex2f(x, y);
    glEnd();
    drawPoint(0.0,0.0); // el origen es la articulacion
}

/****************** CUERPO ****************/
// DIBUJA PIERNA
void drawLeg(float x, float y,float rotateP,float rotateR,float rotateT){
    glPushMatrix();
        glTranslatef(x,0.0,0.0); //muevo el eje de la pierna izq
        glRotatef(rotateP,0,0,1);
        glColor3fv(blue);
        drawLine(0.0, y);
        //rodilla
        glColor3fv(green);
        glPushMatrix();
            glTranslatef(0.0,y,0.0);
            glRotatef(rotateR,0,0,1);
            drawLine(0.0, y);
            //tobillo
            glColor3fv(yellow);
            glPushMatrix();
                glTranslatef(0.0,y,0.0);
                glRotatef(rotateT,0,0,1);
                drawLine(x,0.0);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

// DIBUJA BRAZO
void drawArm(float x, float xt,float rotateB,float rotateC,float rotateM){
    glPushMatrix();
        glTranslatef(xt,3.0,0.0);
        glRotatef(rotateB,0,0,1);
        glColor3fv(blue);
        drawLine(x,0.0);
        //codo
        glColor3fv(green);
        glPushMatrix();
            glTranslatef(x,0.0,0.0);
            glRotatef(rotateC,0,0,1);
            drawLine(x,0.0);
            //muneca
            glColor3fv(yellow);
            glPushMatrix();
                glTranslatef(x,0.0,0.0);
                glRotatef(rotateM,0,0,1);
                drawLine(0.0,1.0);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();  
}

// DIBUJA CABEZA
void drawHead(){
    glPushMatrix();
        glTranslatef(0.0,3.5,0.0); //muevo el eje de la cabeza
        glRotatef(rotC,0,0,1);
        glColor3fv(cyan);
        //ejesCoordenada(1.0); //borrar
        drawCircle(0.0,1.5,1.5);
        drawPoint(0.0,0.0);
    glPopMatrix();
}

// DIBUJA PARTE DE ARRIBA DEL CUERPO
void drawBack(){
    glPushMatrix();
        glTranslatef(0.0,3.5,0.0); //muevo el eje del Torso
        glRotatef(rotT,0,0,1);

        //Torso y hombros
        glBegin(GL_LINES);
            glColor3fv(cyan);
            glVertex2f(0.0, 0.0);
            glVertex2f(0.0, 3.5);
            glColor3fv(magenta);
            glVertex2f(-2.0, 3.0);
            glVertex2f(2.0, 3.0);
        glEnd();
        drawPoint(0.0,0.0);

        // Cabeza
        drawHead();

        // Brazo izquierdo
        drawArm(2.5, 2.0,rotBi,rotCi,rotMi);

        // Brazo derecho
        drawArm(-2.5, -2.0,rotBd,rotCd,rotMd);

    glPopMatrix();
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

    /* Render Grid */
   /* glPushMatrix();
        glColor3f( 0.0f, 0.7f, 0.7f );
        glBegin( GL_LINES );
            zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
            for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ ){
                xLocal = DEF_floorGridScale * loopX;
                glVertex3f( xLocal, -zExtent, 0.0f );
                glVertex3f( xLocal, zExtent,  0.0f );
            }
            xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
            for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ ){
                zLocal = DEF_floorGridScale * loopZ;
                glVertex3f( -xExtent, zLocal, 0.0f );
                glVertex3f(  xExtent, zLocal, 0.0f );
            }
        glEnd();
    glPopMatrix(); */
    /* END Render Grid */

    glTranslatef(0.0,-1.5,0.0); 
    glEnable(GL_POINT_SMOOTH); //para puntos redondos

    // Parte inferior del cuerpo
    glBegin(GL_LINES);
        glColor3fv(cyan);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, 3.5);
        glVertex2f(-1.5, 0.0);
        glVertex2f(1.5, 0.0);
    glEnd();

    // Pierna Izquierda
    drawLeg(1.5, -4.0,rotPi,rotRi,rotTi);

    // Pierna Derecho
    drawLeg(-1.5, -4.0,rotPd,rotRd,rotTd);

    // Parte superior del cuerpo
    drawBack();

    glutSwapBuffers();
}

//Incrementa o decrementa el angulo de la extremidad superior
void supExt(int n){
    switch (area){
        case '1':
            rotPd+=n*ang; break;
        case '2': 
            rotPi+=n*ang; break;
        case '3':
            rotBd+=n*ang; break;
        case '4': 
            rotBi+=n*ang; break;
        case '5':
            rotC+=n*ang;  break;
        default:
        break;
    }
}

//Incrementa o decrementa el angulo de la extremidad del medio
void medExt(int n){
    switch (area){
        case '1':
            rotRd+=n*ang; break;
        case '2': 
            rotRi+=n*ang; break;
        case '3':
            rotCd+=n*ang; break;
        case '4': 
            rotCi+=n*ang; break;
        case '6': 
            rotT+=ang;    break;
        default:
        break;
    }
}

//Incrementa o decrementa el angulo de la extremidad inferior
void infExt(int n){
    switch (area){
        case '1':
            rotTd+=n*ang; break;
        case '2': 
            rotTi+=n*ang; break;
        case '3':
            rotMd+=n*ang; break;
        case '4': 
            rotMi+=n*ang; break;
        default:
        break;
    }
}

void selectExt (unsigned char key, int xmouse, int ymouse){   
    switch (key){
        case 'A': //Rotar extremidad superior anti horario.  
            supExt(1);  break;
        case 'Z': //Rotar extremidad superior horario.  
            supExt(-1); break;
        case 'S': //Rotar extremidad del medio antihorario. 
            medExt(1);  break;
        case 'X': //Rotar extremidad del medio horario.  
            medExt(-1); break;
        case 'D': //Rotar extremidad inferior anti horario.  
            infExt(1);  break; 
        case 'C': //Rotar extremidad inferior horario. 
            infExt(-1); break;
        default:
        break;
    }
   glutPostRedisplay(); 
}


void selectArea (unsigned char key, int xmouse, int ymouse){  
    area=key; 
    switch (key){
        case '1':
            glClearColor (1.0, 0.0, 0.0, 0.0);
        break;
        case '2': 
            glClearColor (0.0, 1.0, 0.0, 0.0);
        break;
        case '3':
            glClearColor (1.0, 0.0, 0.0, 0.0);
        break;
        case '4': 
            glClearColor (0.0, 1.0, 0.0, 0.0);
        break;
        case '5':
            glClearColor (1.0, 0.0, 0.0, 0.0);
        break;
        case '6': 
            glClearColor (0.0, 1.0, 0.0, 0.0);
        break;
        default:
        break;
    }
   glutPostRedisplay(); 
}

int main (int argc, char** argv) {
    area='2';
    infExt(-1);
    area='4';
    infExt(-1);
    supExt(1);
    medExt(-1);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("Opengl");
    glutReshapeFunc(changeViewport);
    glutDisplayFunc(render);

    glutKeyboardFunc(selectArea);
    /*
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW error");
        return 1;
    }*/
    
    glutMainLoop();
    return 0;
}