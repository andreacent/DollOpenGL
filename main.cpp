#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>

using namespace std;

#define DEF_floorGridScale  1.0f
#define DEF_floorGridXSteps 12.0f
#define DEF_floorGridZSteps 12.0f

//INICIALIZO LOS ANGULOS
float   rotD=0.0,rotC=0.0,   //dorso, cabeza
        rotBi=0.0,rotBd=0.0, //brazos
        rotMi=0.0,rotMd=0.0, //muneca
        rotCi=0.0,rotCd=0.0, //codo
        rotPi=0.0,rotPd=0.0, //piernas
        rotRi=0.0,rotRd=0.0, //rodilla
        rotTi=0.0,rotTd=0.0; //tobillo

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

/****************** CUERPO ****************/
// DIBUJA PIERNA
void drawLeg(float x, float y,float rotateP,float rotateR,float rotateT){
    glPushMatrix();
        glTranslatef(x,0.0,0.0); //muevo el eje de la pierna izq
        glRotatef(rotateP,0,0,1);
        glColor3f(0,0,1);
        drawLine(0.0, y);
        //rodilla
        glColor3f(0,1,0);
        glPushMatrix();
            glTranslatef(0.0,y,0.0);
            glRotatef(rotateR,0,0,1);
            drawLine(0.0, y);
            //tobillo
            glColor3f(1,1,0);
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
        glColor3f(0,0,1);
        drawLine(x,0.0);
        //codo
        glColor3f(0,1,0);
        glPushMatrix();
            glTranslatef(x,0.0,0.0);
            glRotatef(rotateC,0,0,1);
            drawLine(x,0.0);
            //muneca
            glColor3f(1,1,0);
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
        glColor3f(0,1,1);
        //ejesCoordenada(1.0); //borrar
        drawCircle(0.0,1.5,1.5);
        drawPoint(0.0,0.0);
    glPopMatrix();
}

// DIBUJA PARTE DE ARRIBA DEL CUERPO
void drawBack(){
    glPushMatrix();
        glTranslatef(0.0,3.5,0.0); //muevo el eje del dorso
        glRotatef(rotD,0,0,1);

        //dorso y hombros
        glBegin(GL_LINES);
            glColor3f(0,1,1);
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
        drawArm(-2.5, -2.0,rotBi,rotCi,rotMi);

        // Brazo derecho
        drawArm(2.5, 2.0,rotBd,rotCd,rotMd);

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
    glEnable(GL_POINT_SMOOTH);

    // Parte inferior del cuerpo
    glBegin(GL_LINES);
        glColor3f(0,1,1);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, 3.5);
        glVertex2f(-1.5, 0.0);
        glVertex2f(1.5, 0.0);
    glEnd();

    // Pierna Izquierda
    drawLeg(-1.5, -4.0,rotPi,rotRi,rotTi);

    // Pierna Derecho
    drawLeg(1.5, -4.0,rotPd,rotRd,rotTd);

    // Parte superior del cuerpo
    drawBack();

    glutSwapBuffers();
}


void selectArea (unsigned char key, int xmouse, int ymouse){   
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
    rotBi=-90.0;
    rotMi=90.0;
    rotPd=90.0;
    rotCd=25.0;
    rotTi=90.0;
    rotTd=-90.0;
    //rotTd=-40.0;

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