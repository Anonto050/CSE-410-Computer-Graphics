#include<bits/stdc++.h>

#ifdef __linux__
    #include<GL/glut.h>
#elif WIN32
    #include <windows.h>
    #include <glut.h>
#endif


using namespace std;

struct Point
{
    double x, y;
  
};

double radius = 0.4;
double minir = 0.2;
double minir2 = 0.05;
double rotation = 0;
double rotation2 = 0;

Point trace, trace2;
double sinemove = 180;

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

void drawCircle()
{
    double theta = 0;
    double prev_x = radius;
    double prev_y = 0;

    while(theta <= 2*3.1416)
    {
        double x = radius * cos(theta);
        double y = radius * sin(theta);

        glBegin(GL_LINES);
        {
            glVertex3f(prev_x, prev_y, 0);
            glVertex3f(x, y, 0);
        }
        glEnd();

        prev_x = x;
        prev_y = y;
        theta += 0.01;
    }
}

void drawMiniCircle(){

    double theta = 0;   
    // double minir = 0.2;

    double prev_x = minir + trace.x;
    double prev_y = 0 + trace.y;

    while (theta <= 360)
    {
        double thetainrad = theta * 3.1416 / 180;
        double x = minir * cos(thetainrad) + trace.x;
        double y = minir * sin(thetainrad) + trace.y;

        glBegin(GL_LINES);
        {
            glColor3f(1, 1, 1);
            glVertex2f(prev_x, prev_y);
            glVertex2f(x, y);
        }
        glEnd();

        prev_x = x;
        prev_y = y;
        theta += 5;

    }

}

void drawMiniCircle2(){

    double theta = 0;

    double prev_x = minir2 + trace2.x;
    double prev_y = 0 + trace2.y;

    while (theta <= 360)
    {
        double thetainrad = theta * 3.1416 / 180;
        double x = minir2 * cos(thetainrad) + trace2.x;
        double y = minir2 * sin(thetainrad) + trace2.y;

        glBegin(GL_LINES);
        {
            glColor3f(1, 1, 1);
            glVertex2f(prev_x, prev_y);
            glVertex2f(x, y);
        }
        glEnd();

        prev_x = x;
        prev_y = y;
        theta += 5;

    }

}

void drawLine(){

    glBegin(GL_LINES);
    {
        glVertex3f(0, 0, 0);
        glVertex3f(trace.x, trace.y, 0);
    }
    glEnd();
 
    glBegin(GL_LINES);
    {
        glVertex3f(trace.x, trace.y, 0);
        glVertex3f(radius+0.2, trace.y, 0);
    }
    glEnd();

    glBegin(GL_LINES);
    {
        glVertex3f(trace.x, trace.y, 0);
        glVertex3f(trace2.x, trace2.y, 0);
    }
    glEnd();

}

void drawWave(){

    double x = 0;
    double startingX = radius + 0.2;
    double phaseOffset = rotation; 

    glBegin(GL_POINTS);
    {
        glColor3f(1, 1, 1);

        for (x = 0; x <= 2 * 3.1416; x += 0.001)
        {
            double y = radius * sin(x + phaseOffset); // Use `x + phaseOffset` to simulate wave movement
            glVertex2f(startingX + x*0.1, y);
        }

    }
    glEnd();
}


void display()
{
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // gluLookAt(
    //     10, 10, 5, 
    //     0, 0, 0, 
    //     0, 1, 0
    //     );

    // axes();
    
    glPushMatrix();
    glTranslatef(-0.3, 0, 0);
    drawMiniCircle2();
    drawMiniCircle();
    drawCircle();
    drawLine();
    drawWave();
    glPopMatrix();

    // glPushMatrix();
    // glTranslatef(-0.3, 0, 0);
    // drawLine();
    // glPopMatrix();

    rotation += 0.0005;
    rotation2 += 0.0012;
    
    if(rotation > 2*3.1416)
        rotation = 0;

    if(rotation2 > 2*3.1416)
        rotation2 = 0;    
    
    trace.x = radius * cos(rotation);
    trace.y = radius * sin(rotation);

    trace2.x = minir*cos(rotation2) + trace.x;
    trace2.y = minir*sin(rotation2) + trace.y;


    glFlush();
}

void init(){
    printf("Do your initialization here\n");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

    trace.x = radius;
    trace.y = 0;

    trace2.x = minir + radius; 
    trace2.y = 0;
    
}

void idle(){
    glutPostRedisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Wave");

    init();  

    glutDisplayFunc(display);
    // glutTimerFunc(0, animate, 0);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}