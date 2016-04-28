#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>

using namespace std;

#define DEF_floorGridScale  1.0f
#define DEF_floorGridXSteps 10.0f
#define DEF_floorGridZSteps 10.0f

void ejesCoordenada(float w) {
    
    glLineWidth(w);
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
        glVertex2f(0,10);
        glVertex2f(0,-10);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(10,0);
        glVertex2f(-10,0);
    glEnd();

    glLineWidth(w-1.0);
    int i;
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
        for(i = -10; i <=10; i++){
            if (i!=0) {     
                if ((i%2)==0){  
                    glVertex2f(i,0.4);
                    glVertex2f(i,-0.4);

                    glVertex2f(0.4,i);
                    glVertex2f(-0.4,i);
                }else{
                    glVertex2f(i,0.2);
                    glVertex2f(i,-0.2);

                    glVertex2f(0.2,i);
                    glVertex2f(-0.2,i);
                }
            }
        }
    glEnd();

    glLineWidth(1.0);
}

void changeViewport(int w, int h) {
    float aspectratio;
    aspectratio = (float) w / (float) h ;

    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (w <= h){
        glOrtho(-10.0, 10.0, -10.0/aspectratio, 10.0/aspectratio, -1.0, 1.0); 
    }else{
        glOrtho(-10.0*aspectratio, 10.0*aspectratio, -10.0, 10.0, -1.0, 1.0);
    }
}

//  DIBUJA CIRCUNFERENCIA
void drawCircle(float px, float py, float radio) {
    float calx,caly;
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
    glPointSize(1.0);
}

// DIBUJA RECTAS
void drawLine(float x, float y){
    glBegin(GL_LINES);
        glVertex2f(0.0, 0.0);
        glVertex2f(x, y);
    glEnd();
    drawPoint(0.0,0.0); // el origen es la articulacion
}

// DIBUJA PIERNA
void drawLeg(float x, float y){
    glPushMatrix();
        glTranslatef(x,0.0,0.0); //muevo el eje de la pierna izq
        glColor3f(0,0,1);
        drawLine(0.0, y);
        //rodilla
        glColor3f(0,1,0);
        glPushMatrix();
            glTranslatef(0.0,y,0.0);
            drawLine(0.0, y);
            //tobillo
            glColor3f(1,1,0);
            glPushMatrix();
                glTranslatef(0.0,y,0.0);
                drawLine(x,0.0);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

// DIBUJA BRAZO
void drawArm(float x, float xt){
    glPushMatrix();
        glTranslatef(xt,3.0,0.0);
        glColor3f(0,0,1);
        drawLine(x,0.0);
        //codo
        glColor3f(0,1,0);
        glPushMatrix();
            glTranslatef(x,0.0,0.0);
            drawLine(x,0.0);
            //muneca
            glColor3f(1,1,0);
            glPushMatrix();
                glTranslatef(x,0.0,0.0);
                drawLine(0.0,1.0);
            glPopMatrix();
        glPopMatrix();
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

    glTranslatef(0.0,-1.0,0.0); 

    // Parte inferior del cuerpo
    glBegin(GL_LINES);
        glColor3f(0,1,1);
        glVertex2f(0.0, 0.0);
        glVertex2f(0.0, 3.5);
        glVertex2f(-1.5, 0.0);
        glVertex2f(1.5, 0.0);
    glEnd();

    // Pierna Izquierda
    drawLeg(-1.5, -4.0);

    // Pierna Derecho
    drawLeg(1.5, -4.0);

    // Parte superior del cuerpo
    glPushMatrix();
        glTranslatef(0.0,3.5,0.0); //muevo el eje del dorso

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
        glPushMatrix();
            glTranslatef(0.0,3.5,0.0); //muevo el eje de la cabeza
            glColor3f(0,1,1);
            //ejesCoordenada(1.0); //borrar
            drawCircle(0.0,1.5,1.5);
            drawPoint(0.0,0.0);
        glPopMatrix();

        // Brazo izquierdo
        drawArm(-2.5, -2.0);

        // Brazo derecho
        drawArm(2.5, 2.0);

    glPopMatrix();

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
   glutPostRedisplay(); //request display() call ASAP
}

int main (int argc, char** argv) {

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