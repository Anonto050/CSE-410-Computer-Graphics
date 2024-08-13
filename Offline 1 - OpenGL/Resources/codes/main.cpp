#include <bits/stdc++.h>
using namespace std;

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

int counter = 0;

void axes()
{
        glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
    
}

void square(double a){
    
    glBegin(GL_QUADS);
    {
        glVertex3f(a, a, 0);
        glVertex3f(a, -a, 0);
        glVertex3f(-a, -a, 0);
        glVertex3f(-a, a, 0);
    }glEnd();
}

int animate;

void display()
{
    // glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        10, 10, 5, 
        0, 0, 0, 
        0, 1, 0
        );

    axes();
    glColor3f(1, 1, 1);
    // square(.25);


    for(int i = 0; i < 4; i++){

        glPushMatrix();
        glRotatef( i * 90 , 1, 0, 0);
        glTranslatef(0, 0, 2);
        glColor3f(1, 0, 0);
        square(2);
        glPopMatrix();   

        glPushMatrix();
        glRotatef( i * 90 , 0, 1, 0);
        glTranslatef(0, 0, 2);
        glColor3f(0, 1, 0);
        square(2);
        glPopMatrix();  

        // printf("angle : %d\n", i * 90);
    }



    // glFlush();
    glutSwapBuffers();
}

void init()
{
    // glClearColor(0.1f, .0f, 0.0f, 1.0f); // Set background color to black and opaque
    animate = 1;


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);

}

// void idle()
// {
//     // printf("Currently we don't have any job!!!\n");

//     glutPostRedisplay();
// }

void Timer(int value){
    // printf("We are in Timer function. couter : %d\n", ++counter);
    if(animate){
        counter++;
    }

    glutPostRedisplay();
    glutTimerFunc(10, Timer, 0);
}

void keyboardHandler(unsigned char key, int x, int y){
    switch(key){
        case 'V':
            printf("V pressed\n");
            animate = !animate;
            break;
        default:
            printf("ulta palta key press kora bondho koren!\n");
            break;
    }
}

int main(int argc, char **argv)
{
    printf("Hello World\n");
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(480, 480);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL Demo");

    init();

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboardHandler);

    // glutIdleFunc(idle);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();
    return 0;
}