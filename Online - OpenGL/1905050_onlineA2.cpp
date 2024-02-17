#include <bits/stdc++.h>

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

using namespace std;
int drawaxes;

double radius = 50;
double square_side = 10;

double rotation = 0;
double rotation2 = 0;
double inc_rotation = 0;

void init()
{

    printf("Do your initialization here\n");
    drawaxes = 1;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
}

void drawAxes()
{
    if (drawaxes == 1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f(100, 0, 0);
            glVertex3f(-100, 0, 0);

            glVertex3f(0, -100, 0);
            glVertex3f(0, 100, 0);

            glVertex3f(0, 0, 100);
            glVertex3f(0, 0, -100);
        }
        glEnd();
    }
}

// a openGL integer
GLint counter = 0;

void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'K':
        /* code */
        printf("K pressed\n");
        break;

    case '1':

        if (rotation < 90)
        {
            rotation += 5;
            inc_rotation += 0.002;
        }

        break;

    case '2':
        if (rotation > 0)
        {
            rotation -= 5;
            inc_rotation -= 0.002;
        }
        break;

    default:
        printf("We don't know what you pressed\n");
        break;
    }
}
void drawSquare(double a)
{
    glBegin(GL_QUADS);
    {
        glVertex3f(a, a, 0);
        glVertex3f(a, -a, 0);
        glVertex3f(-a, -a, 0);
        glVertex3f(-a, a, 0);
    }
    glEnd();
}

void drawLines()
{
    glBegin(GL_LINES);
    {
        glVertex3f(0, 0, 0);
        glVertex3f(square_side / 2, 0, -40);
    }
    glEnd();

    glBegin(GL_LINES);
    {
        glVertex3f(0, 0, 0);
        glVertex3f(-square_side / 2, 0, -40);
    }
    glEnd();
}

void drawCircle()
{

    double theta = 0;
    double prev_x = radius;
    double prev_y = 0;

    while (theta <= 2 * 3.1416)
    {
        double x = radius * cos(theta);
        double y = radius * sin(theta);

        glBegin(GL_LINES);
        {
            glColor3f(1, 1, 1);
            glVertex3f(prev_x, prev_y, 0);
            glVertex3f(x, y, 0);
        }
        glEnd();

        prev_x = x;
        prev_y = y;
        theta += 0.01;
    }
}

void drawPlate()
{

    glPushMatrix();

    glColor3f(1, 1, 1);
    glTranslatef(0, 0, -40);
    drawSquare(square_side);

    glPopMatrix();

    glPushMatrix();
    drawLines();
    glPopMatrix();
}

void drawPositionedPlate()
{
    glPushMatrix();
    glTranslatef(radius, 0, 0);
    glRotatef(-rotation, 0, 1, 0);
    drawPlate();
    glPopMatrix();
}

void drawPlates()
{
    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();
        glRotatef(i * 60, 0, 0, 1);
        drawPositionedPlate();
        glPopMatrix();
    }
}

void display()
{
    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(100, 100, 100, 0, 0, 0, 0, 0, 1);
    drawAxes();

    rotation2 += inc_rotation;

    if (rotation2 > 2 * 3.1416)
        rotation2 = 0;

    glPushMatrix();
    glRotatef(rotation2 * 180 / 3.1416, 0, 0, 1);
    drawPlates();
    glPopMatrix();
    drawCircle();

    glutSwapBuffers();
}

void idle()
{

    counter++;
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(450, 450);     // Set the window's initial width & height
    glutInitWindowPosition(750, 250); // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Merry-Go-Round");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutIdleFunc(idle);
    init();

    glutMainLoop();
    return 0;
}
