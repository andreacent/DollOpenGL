#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>

using namespace std;

#define DEF_floorGridScale  1.0f
#define DEF_floorGridXSteps 12.0f
#define DEF_floorGridZSteps 12.0f

/************************* ANGULOS *************************/
float   rotT=0.0,rotC=0.0,   //Torso, cabeza
        rotBi=0.0,rotBd=0.0, //hombro
        rotMi=0.0,rotMd=0.0, //muneca
        rotCi=0.0,rotCd=0.0, //codo
        rotPi=0.0,rotPd=0.0, //pierna
        rotRi=0.0,rotRd=0.0, //rodilla
        rotTi=0.0,rotTd=0.0, //tobillo
        ang = 30.0;          //angulo

char    area;

/************************* COLORES *************************/
float   rBi, rBd, bBi, bBd, // brazo
        rPi, rPd, bPi, bPd,  // pierna
        rCi, rCd, bCi, bCd, // codo
        rRi, rRd, bRi, bRd, // rodilla
        bMi, bMd, // muneca
        bTi, bTd, // tobillo
        rC, // cabeza
        rT; // torso

// INICIALIZO LOS COLORES
void initializeColor(){
    rBi=0; rBd=0; bBi=0; bBd=0;
    rPi=0; rPd=0; bPi=0; bPd=0;
    rCi=0.2; rCd=0.2; bCi=0.4; bCd=0.4;
    rRi=0.2; rRd=0.2; bRi=0.4; bRd=0.4;
    bMi=0; bMd=0; bTi=0; bTd=0;
    rC=0; rT=0; 
}

// INICIALIZO LOS COLORES
void changeColor(){
    initializeColor();
    switch (area){
        case '1': rPd=1; bPd=1; rRd=1; bRd=1; bTd=1; break;
        case '2': rPi=1; bPi=1; rRi=1; bRi=1; bTi=1; break;
        case '3': rBd=1; bBd=1; bMd=1; rCd=1; bCd=1; break;
        case '4': rBi=1; bBi=1; bMi=1; rCi=1; bCi=1; break;
        case '5': rC=1;  break;
        case '6': rT=1;  break;
        default: break;
    }
    glutPostRedisplay();
}

//  DIBUJA CIRCUNFERENCIA
void drawCircle(float px, float py, float radio) {
    float x,y;
    glPointSize(2.0);
    glBegin(GL_POINTS);
        for(double i=0.0; i<10; i+=0.001){
            x=radio*cos(i)+px;
            y=radio*sin(i)+py;
            glVertex2f(x,y);
        }
    glEnd();
}

//  DIBUJA UN PUNTO (ARTICULACION)
void drawPoint(float x, float y){
    glColor3f(1,0,0);
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

/***************************** DIBUJAR CUERPO ********************************/
// DIBUJA PIERNA
void drawLeg(float x, float y,float rotateP,float rotateR,float rotateT,
             float rP,float rR,float bP,float bR,float bT){
    glPushMatrix();
        glTranslatef(x,0.0,0.0); //muevo el eje de la pierna izq
        glRotatef(rotateP,0,0,1);
        glColor3f(rP,1,bP);
        drawLine(0.0, y);
        //rodilla
        glColor3f(rR,1,bR);
        glPushMatrix();
            glTranslatef(0.0,y,0.0);
            glRotatef(rotateR,0,0,1);
            drawLine(0.0, y);
            //tobillo
            glColor3f(1,1,bT);
            glPushMatrix();
                glTranslatef(0.0,y,0.0);
                glRotatef(rotateT,0,0,1);
                drawLine(x,0.0);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

// DIBUJA BRAZO
void drawArm(float x, float xt,float rotateB,float rotateC,float rotateM,
             float rB,float rC,float bB,float bC,float bM){
    glPushMatrix();
        glTranslatef(xt,3.0,0.0);
        glRotatef(rotateB,0,0,1);
        glColor3f(rB,1,bB);
        drawLine(x,0.0);
        //codo
        glColor3f(rC,1,bC);
        glPushMatrix();
            glTranslatef(x,0.0,0.0);
            glRotatef(rotateC,0,0,1);
            drawLine(x,0.0);
            //muneca
            glColor3f(1,1,bM);
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
        glColor3f(rC,1,1);
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
            glColor3f(rT,1,1);
            glVertex2f(0.0, 0.0);
            glVertex2f(0.0, 3.5);
            glColor3f(0,0,1);
            glVertex2f(-2.0, 3.0);
            glVertex2f(2.0, 3.0);
        glEnd();
        drawPoint(0.0,0.0);

        // Cabeza
        drawHead();

        // Brazo izquierdo
        drawArm(2.5,2.0,rotBi,rotCi,rotMi,rBi,rCi,bBi,bCi,bMi);

        // Brazo derecho
        drawArm(-2.5,-2.0,rotBd,rotCd,rotMd,rBd,rCd,bBd,bCd,bMd);

    glPopMatrix();
}

/************************** Viewport **************************/
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

/************************** Display **************************/
void render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

    glTranslatef(0.0,-1.5,0.0); 
    glEnable(GL_POINT_SMOOTH); //para puntos redondos

    // Parte inferior del cuerpo
    glBegin(GL_LINES);
        glColor3f(rT,1,1);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, 3.5);
        glColor3f(0,1,1);
        glVertex2f(-1.5, 0.0);
        glVertex2f(1.5, 0.0);
    glEnd();

        // Pierna Izquierda
    drawLeg(1.5, -4.0,rotPi,rotRi,rotTi,rPi,rRi,bPi,bRi,bTi);

        // Pierna Derecho
    drawLeg(-1.5, -4.0,rotPd,rotRd,rotTd,rPd,rRd,bPd,bRd,bTd);

    // Parte superior del cuerpo
    drawBack();

    glutSwapBuffers();
}

/******************************* ROTACION *****************************/
//Incrementa o decrementa el angulo de la extremidad superior
void supExt(int n){
    switch (area){
        case '1': rotPd+=n*ang; break;
        case '2': rotPi+=n*ang; break;
        case '3': rotBd+=n*ang; break;
        case '4': rotBi+=n*ang; break;
        case '5': rotC +=n*ang; break;
        default: break;
    }
}

//Incrementa o decrementa el angulo de la extremidad del medio
void medExt(int n){
    switch (area){
        case '1': rotRd+=n*ang; break;
        case '2': rotRi+=n*ang; break;
        case '3': rotCd+=n*ang; break;
        case '4': rotCi+=n*ang; break;
        case '6': rotT +=n*ang; break;
        default: break;
    }
}

//Incrementa o decrementa el angulo de la extremidad inferior
void infExt(int n){
    switch (area){
        case '1': rotTd+=n*ang; break;
        case '2': rotTi+=n*ang; break;
        case '3': rotMd+=n*ang; break;
        case '4': rotMi+=n*ang; break;
        default: break;
    }
}

/******************************* KEYBOARD *****************************/
void controlKey (unsigned char key, int xmouse, int ymouse){   
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
            area = key; changeColor(); 
        break;
    }
    //initializeColor();
    glutPostRedisplay(); 
}

/******************************* MAIN *****************************/
int main (int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("Opengl");
    glutReshapeFunc(changeViewport);

    initializeColor();
    glLineWidth(2.0);
    glutDisplayFunc(render);

    glutKeyboardFunc(controlKey);

    /*
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW error");
        return 1;
    }*/
    
    glutMainLoop();
    return 0;
}