#include<bits/stdc++.h>

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif


using namespace std;
int drawaxes;

void init(){
    printf("Do your initialization here\n");
    drawaxes = 1;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
    

}

void drawAxes()
{
	if(drawaxes==1)
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
}


// a openGL integer
GLint counter = 0;

void keyboardListener(unsigned char key,int x, int y){
    switch (key)
    {
    case 'K':
        /* code */
        printf("K pressed\n");
        break;
    
    default:
        printf("We don't know what you pressed\n");
        break;
    }
}
void drawSquare(double a){
    glBegin(GL_QUADS);{
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }glEnd();

}

double _rand(){
    return (double)rand() / RAND_MAX;
}

void display(){
    printf("Display function called for %d times\n", counter);
    
    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(100,100,100,	0,0,0,	0,0,1);
    drawAxes();

    glBegin(GL_LINES);{
        glColor3f(1.0f, 1.0f, 1.0f); // Green
        glVertex2f(-1.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);
        glVertex2f(0.0f, -1.0f);
        glVertex2f(0.0f, 1.0f);
       
    }glEnd();


    for(int i = 0; i < 8; i++){
        glPushMatrix();
        glRotatef(i * 90, 1, 0, 0);
        glTranslatef(0, 0, 20);
        glColor3f(1, 0, 0);
        // glColor3f(_rand(), _rand(), _rand()); // Red
        drawSquare(20);
        glPopMatrix();
    }
    
   


    // glFlush();
    glutSwapBuffers();

}

void idle(){

    counter++;
    glutPostRedisplay();
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitWindowSize(450, 450);   // Set the window's initial width & height
    glutInitWindowPosition(750, 250); // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Test");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutIdleFunc(idle);
    init();


    glutMainLoop();
    return 0;

}