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
GLfloat const blue[3]    = {0,0,1}, 
              cyan[3]    = {0,1,1}, 
              white[3]   = {1,1,1}, 
              green[3]   = {0,1,0}, 
              yellow[3]  = {1,1,0}; 

GLfloat const   *colT,*colC,*colBi,*colBd,*colMi,
                *colMd,*colCi,*colCd,*colPi,*colPd,
                *colRi,*colRd,*colTi,*colTd;

// INICIALIZO LOS COLORES
void initializeColor(){
    colT  = cyan; colC = cyan; 
    colBi = blue; colBd = blue; colPi = blue; colPd = blue; 
    colMi = yellow; colMd = yellow; colTi = yellow; colTd = yellow; 
    colCi = green; colCd = green; colRi = green; colRd = green;
}

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
             GLfloat const cP[3], GLfloat const cR[3], GLfloat const cT[3]){
    glPushMatrix();
        glTranslatef(x,0.0,0.0); //muevo el eje de la pierna izq
        glRotatef(rotateP,0,0,1);
        glColor3fv(cP);
        drawLine(0.0, y);
        //rodilla
        glColor3fv(cR);
        glPushMatrix();
            glTranslatef(0.0,y,0.0);
            glRotatef(rotateR,0,0,1);
            drawLine(0.0, y);
            //tobillo
            glColor3fv(cT);
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
             GLfloat const cB[3], GLfloat const cC[3], GLfloat const cM[3]){
    glPushMatrix();
        glTranslatef(xt,3.0,0.0);
        glRotatef(rotateB,0,0,1);
        glColor3fv(cB);
        drawLine(x,0.0);
        //codo
        glColor3fv(cC);
        glPushMatrix();
            glTranslatef(x,0.0,0.0);
            glRotatef(rotateC,0,0,1);
            drawLine(x,0.0);
            //muneca
            glColor3fv(cM);
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
        glColor3fv(colC);
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
            glColor3fv(colT);
            glVertex2f(0.0, 0.0);
            glVertex2f(0.0, 3.5);
            glColor3f(1,0,1);
            glVertex2f(-2.0, 3.0);
            glVertex2f(2.0, 3.0);
        glEnd();
        drawPoint(0.0,0.0);

        // Cabeza
        drawHead();

        // Brazo izquierdo
        drawArm(2.5, 2.0,rotBi,rotCi,rotMi,colBi,colCi,colMi);

        // Brazo derecho
        drawArm(-2.5, -2.0,rotBd,rotCd,rotMd,colBd,colCd,colMd);

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
    initializeColor();
    glTranslatef(0.0,-1.5,0.0); 
    glEnable(GL_POINT_SMOOTH); //para puntos redondos

    // Parte inferior del cuerpo
    glBegin(GL_LINES);
        glColor3fv(colT);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, 3.5);
        glColor3fv(cyan);
        glVertex2f(-1.5, 0.0);
        glVertex2f(1.5, 0.0);
    glEnd();

    // Pierna Izquierda
    drawLeg(1.5, -4.0,rotPi,rotRi,rotTi,colPi,colRi,colTi);

    // Pierna Derecho
    drawLeg(-1.5, -4.0,rotPd,rotRd,rotTd,colPd,colRd,colTd);

    // Parte superior del cuerpo
    drawBack();

    glutSwapBuffers();
}

/******************************* MOVIMIENTO *****************************/
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

/******************************* KEYBOARD *****************************/
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
    //initializeColor();
    glutPostRedisplay(); 
}

void selectArea (unsigned char key, int xmouse, int ymouse){  
    //area=key; 
    switch (key){
        case '1':
            colPd = white; colRd = white; colTd = white;
        break;
        case '2': 
            colPi = white; colRi = white; colTi = white;
        break;
        case '3':
            colBd = white; colBd = white; colBd = white;
        break;
        case '4': 
            colBi = white; colBi = white; colBi = white;
        break;
        case '5':
            colC = white; 
        break;
        case '6': 
            colT = white; 
        break;
        default:
        break;
    } 
    glutPostRedisplay(); 
}

int main (int argc, char** argv) {
    area='2';

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("Opengl");
    glutReshapeFunc(changeViewport);
    glutDisplayFunc(render);

    glutKeyboardFunc(selectArea);
    glutKeyboardFunc(selectExt);

    /*
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW error");
        return 1;
    }*/
    
    glutMainLoop();
    return 0;
}