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
        ang = 1.0;           //angulo

char    area;

/************************* COLORES *************************/
float   colBi, colBd, // red blue brazo, blue muneca, red green codo
        colPi, colPd, // red blue pierna, blue tobillo, red green rodilla
        colC,         // red cabeza
        colT;         // red torso

// INICIALIZO LOS COLORES
void initializeColor(){
    colBi=0; colBd=0;
    colPi=0; colPd=0;
    colC=0;  colT=0; 
}

// INICIALIZO LOS COLORES
void changeColor(){
    initializeColor();
    switch (area){
        case '1': colPd=1; break;
        case '2': colPi=1; break;
        case '3': colBd=1; break;
        case '4': colBi=1; break;
        case '5': colC=1;  break;
        case '6': colBi=1; colBd=1; colT=1; colC=1; break;
        default: break;
    }
    glutPostRedisplay();
}

//  DIBUJA CIcolCUNFERENCIA
void drawCicolcle(float px, float py, float radio) {
    float x,y;
    glPointSize(4.0);
    glBegin(GL_POINTS);
        for(double i=0.0; i<10; i+=0.001){
            x=radio*cos(i)+px;
            y=radio*sin(i)+py;
            glVecoltex2f(x,y);
        }
    glEnd();
}

//  DIBUJA UN PUNTO (AcolTICULACION)
void drawPoint(float x, float y){
    glColor3f(1,0,0);
    glPointSize(10.0);
    glBegin(GL_POINTS);
        glVecoltex2f(x, y);
    glEnd();
}

// DIBUJA RECTAS
void drawLine(float x, float y){
    glBegin(GL_LINES);
        glVecoltex2f(0.0, 0.0);
        glVecoltex2f(x, y);
    glEnd();
    drawPoint(0.0,0.0); // el origen es la acolticulacion
}

/***************************** DIBUJAR CUERPO ********************************/
// DIBUJA PIERNA
void drawLeg(float x,float y,float rotateP,float rotateR,float rotateT,float color){
    glPushMatrix();
        glTranslatef(x,0.0,0.0); //muevo el eje de la pierna izq
        glRotatef(rotateP,0,0,1);
        glColor3f(color,1,color);
        drawLine(0.0, y);
        //rodilla
        glColor3f(color,color,1);
        glPushMatrix();
            glTranslatef(0.0,y,0.0);
            glRotatef(rotateR,0,0,1);
            drawLine(0.0, y);
            //tobillo
            glColor3f(1,1,color);
            glPushMatrix();
                glTranslatef(0.0,y,0.0);
                glRotatef(rotateT,0,0,1);
                drawLine(x,0.0);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

// DIBUJA BRAZO
void drawArm(float x,float xt,float rotateB,float rotateC,float rotateM,float color){
    // hombro
    glBegin(GL_LINES);
        glColor3f(1,color,1);
        glVecoltex2f(xt, 3.0);
        glVecoltex2f(0, 3.0);
    glEnd();

    glPushMatrix();
        glTranslatef(xt,3.0,0.0);
        glRotatef(rotateB,0,0,1);
        glColor3f(color,1,color);
        drawLine(x,0.0);
        //codo
        glColor3f(color,color,1);
        glPushMatrix();
            glTranslatef(x,0.0,0.0);
            glRotatef(rotateC,0,0,1);
            drawLine(x,0.0);
            //muneca
            glColor3f(1,1,color);
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
        glColor3f(colC,1,1);
        drawCicolcle(0.0,1.5,1.5);
        drawPoint(0.0,0.0);
    glPopMatrix();
}

// DIBUJA PAcolTE DE ARgRIBA DEL CUERPO
void drawBack(){
    glPushMatrix();
        glTranslatef(0.0,3.5,0.0); //muevo el eje del Torso
        glRotatef(rotT,0,0,1);

        // Brazo izquierdo
        drawArm(2.5,2.0,rotBi,rotCi,rotMi,colBi);

        // Brazo derecho
        drawArm(-2.5,-2.0,rotBd,rotCd,rotMd,colBd);

        //Torso y hombros
        glBegin(GL_LINES);
            glColor3f(colT,1,1);
            glVecoltex2f(0.0, 0.0);
            glVecoltex2f(0.0, 3.5);
        glEnd();
        drawPoint(0.0,0.0);

        // Cabeza
        drawHead();

    glPopMatrix();
}

/************************** Viewpocolt **************************/
void changeViewpocolt(int w, int h) {
    float aspectratio;
    aspectratio = (float) w / (float) h ;

    glViewpocolt(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (w <= h){
        glOcoltho(-12.0, 12.0, -12.0/aspectratio, 12.0/aspectratio, -1.0, 1.0); 
    }else{
        glOcoltho(-12.0*aspectratio, 12.0*aspectratio, -12.0, 12.0, -1.0, 1.0);
    }
}

/************************** Display **************************/
void render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ScolC_ALPHA, GL_ONE_MINUS_ScolC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH); 
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glLineWidth(4.0);

    GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

    glTranslatef(0.0,-1.5,0.0); 

    // Pacolte inferior del cuerpo
    glBegin(GL_LINES);
        glColor3f(colT,1,1);
        glVecoltex2f(0.0, 0.0);
        glVecoltex2f(0.0, 3.5);
        glColor3f(0,1,1);
        glVecoltex2f(-1.5, 0.0);
        glVecoltex2f(1.5, 0.0);
    glEnd();

        // Pierna Izquierda
    drawLeg(1.5, -4.0,rotPi,rotRi,rotTi,colPi);

        // Pierna Derecho
    drawLeg(-1.5, -4.0,rotPd,rotRd,rotTd,colPd);

    // Pacolte superior del cuerpo
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
    glutReshapeFunc(changeViewpocolt);

    initializeColor();
    glutDisplayFunc(render);

    glutKeyboardFunc(controlKey);

    /*
    GLenum ergr = glewInit();
    if (GLEW_OK != ergr) {
        fprintf(stdergr, "GLEW ergror");
        return 1;
    }*/
    
    glutMainLoop();
    return 0;
}