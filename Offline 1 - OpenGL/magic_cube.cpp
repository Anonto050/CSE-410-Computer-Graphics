#include <bits/stdc++.h>

#include "constants.h"

#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

using namespace std;
int drawaxes;

// bool isAxisOn = true;
bool isCylinderOn = true;
bool isOctahedronOn = true;
bool isSphereOn = true;

struct point
{
    double x, y, z;
};

// Here we will be storing the camera position, lookAt position and the up vector.
struct point pos, u, r, l;

struct point crossProduct(struct point a, struct point b)
{
    struct point ret;
    ret.x = a.y * b.z - a.z * b.y;
    ret.y = a.z * b.x - a.x * b.z;
    ret.z = a.x * b.y - a.y * b.x;
    return ret;
}

struct point normalize(struct point a)
{
    struct point ret;
    double magnitude = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    ret.x = a.x / magnitude;
    ret.y = a.y / magnitude;
    ret.z = a.z / magnitude;
    return ret;
}

void init()
{
    drawaxes = 1;

    // Initialize camera
    pos.x = 100.0;
    pos.y = 100.0;
    pos.z = 0.0;

    // Initialize lookAt
    l.x = -1 / sqrt(2);
    l.y = -1 / sqrt(2);
    l.z = 0.0;

    // Initialize up vector
    u.x = 0.0;
    u.y = 0.0;
    u.z = 1.0;

    // Initialize right vector
    r.x = -1 / sqrt(2);
    r.y = 1 / sqrt(2);
    r.z = 0.0;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
}

// Transitions

void transition_Sphere_To_Octahedron()
{
    if (TRIANGLE_SCALE < 1)
    {
        // Triangle Scale
        TRIANGLE_SCALE += 1.0 / TRANSISTION_STEPS;
        Basic_Triangle_Centre_X -= Triangle_Centre_X / TRANSISTION_STEPS;
        Basic_Triangle_Centre_Y -= Triangle_Centre_Y / TRANSISTION_STEPS;
        Basic_Triangle_Centre_Z -= Triangle_Centre_Z / TRANSISTION_STEPS;

        // Sphere Scale
        SPHERE_SCALE -= 1.0 / TRANSISTION_STEPS;
        Sphere_transX += LENGTH / TRANSISTION_STEPS;

        // Cylinder Scale

        CYLINDER_HEIGHT += MAX_CYLINDER_HEIGHT / TRANSISTION_STEPS;
        CYLINDER_TRANSLATION += CYLINDER_MAX_Translation / TRANSISTION_STEPS;
    }
}

void transition_Octahedron_To_Sphere()
{
    if (TRIANGLE_SCALE > 0)
    {

        // Triangle Scale
        TRIANGLE_SCALE -= 1.0 / TRANSISTION_STEPS;
        Basic_Triangle_Centre_X += Triangle_Centre_X / TRANSISTION_STEPS;
        Basic_Triangle_Centre_Y += Triangle_Centre_Y / TRANSISTION_STEPS;
        Basic_Triangle_Centre_Z += Triangle_Centre_Z / TRANSISTION_STEPS;

        // Sphere Scale
        SPHERE_SCALE += 1.0 / TRANSISTION_STEPS;
        Sphere_transX -= LENGTH / TRANSISTION_STEPS;

        // Cylinder Scale
        CYLINDER_HEIGHT -= MAX_CYLINDER_HEIGHT / TRANSISTION_STEPS;
        CYLINDER_TRANSLATION -= CYLINDER_MAX_Translation / TRANSISTION_STEPS;
    }
}

void drawAxes(double axisLength)
{
    if (drawaxes == 1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f(axisLength, 0, 0);
            glVertex3f(-axisLength, 0, 0);

            glVertex3f(0, -axisLength, 0);
            glVertex3f(0, axisLength, 0);

            glVertex3f(0, 0, axisLength);
            glVertex3f(0, 0, -axisLength);
        }
        glEnd();
    }
}

// Octahedron Drawing

void drawTriangle()
{
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(LENGTH, 0, 0);
        glVertex3f(0, LENGTH, 0);
        glVertex3f(0, 0, LENGTH);
    }
    glEnd();
}

void drawHalfOctahedron()
{
    GLdouble rotatationAngle = 90.0;
    int surface = 1;

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotated(rotatationAngle * i, 0, 0, 1);
            // glRotated(45, 0, 1, 0);

            if (surface)
            {
                glColor3f(0.302, 0.753, 0.714);
                surface = !surface;
            }
            else
            {
                glColor3f(0.965, 0.427, 0.635);
                surface = !surface;
            }

            glTranslated(Basic_Triangle_Centre_X, Basic_Triangle_Centre_Y, Basic_Triangle_Centre_Z);
            glScaled(TRIANGLE_SCALE, TRIANGLE_SCALE, TRIANGLE_SCALE);

            drawTriangle();
        }
        glPopMatrix();
    }
}

void drawOctahedron()
{
    glPushMatrix();
    {
        // glTranslated(0, 0, -50);
        drawHalfOctahedron();
    }
    glPopMatrix();

    glPushMatrix();
    {
        // glTranslated(0, 0, -50);
        glRotated(180, 0, 1, 0);
        drawHalfOctahedron();
    }
    glPopMatrix();
}

// Cylinder Drawing

void drawBasicCylinder(int sectorCount)
{
    GLfloat present_x, present_y;
    GLfloat prev_x = CYLINDER_RADIUS, prev_y = 0.0;
    GLfloat angle = CYLINDER_ANGLE / sectorCount;

    glPushMatrix();

    // Yellow color for the cylinder
    glColor3f(1.0, 1.0, 0.0);

    glBegin(GL_QUADS);
    {
        for (int i = 0; i <= sectorCount; i++)
        {

            present_x = CYLINDER_RADIUS * cos(angle * i);
            present_y = CYLINDER_RADIUS * sin(angle * i);

            glVertex3f(present_x, present_y, CYLINDER_HEIGHT / 2);
            glVertex3f(present_x, present_y, -CYLINDER_HEIGHT / 2);

            glVertex3f(prev_x, prev_y, -CYLINDER_HEIGHT / 2);
            glVertex3f(prev_x, prev_y, CYLINDER_HEIGHT / 2);

            prev_x = present_x;
            prev_y = present_y;

            // printf("%f %f\n", present_x, present_y);
        }
    }
    glEnd();

    glPopMatrix();
}

void drawPlacedCylinder()
{
    glPushMatrix();
    {
        glRotated(45, 0, 1, 0);
        glTranslated(CYLINDER_TRANSLATION, 0, 0);
        glScaled(SPHERE_SCALE, SPHERE_SCALE, 1);
        drawBasicCylinder(100);
    }
    glPopMatrix();
}

void drawHalfCylinderSet()
{
    glPushMatrix();
    {
        for (int i = 0; i < 4; i++)
        {
            glRotated(90 * i, 0, 0, 1);
            drawPlacedCylinder();
        }
    }
    glPopMatrix();
}

void drawCylinderSet()
{
    glPushMatrix();
    {
        drawHalfCylinderSet();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glRotated(180, 0, 1, 0);
        drawHalfCylinderSet();
    }
    glPopMatrix();

    // Draw Cylinders of xy plane
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        {
            glRotated(90 * i, 0, 0, 1);
            glRotated(90, 1, 0, 0);
            drawPlacedCylinder();
        }
        glPopMatrix();
    }
}

// Sphere Drawing

// generate vertices for +X face only by intersecting 2 circular planes
// (longitudinal and latitudinal) at the given longitude/latitude angles

void drawPartialSphere(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    // std::vector<float> vertices;
    float n1[3]; // normal of longitudinal plane rotating along Y-axis
    float n2[3]; // normal of latitudinal plane rotating along Z-axis
    float v[3];  // direction vector intersecting 2 planes, n1 x n2
    float a1;    // longitudinal angle along Y-axis
    float a2;    // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // 2D array of points
    // 1st index for longitude, 2nd index for latitude
    struct point spherePoints[pointsPerRow][pointsPerRow];

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for (unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for (unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = CYLINDER_RADIUS / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // Add points to the array
            spherePoints[i][j].x = v[0];
            spherePoints[i][j].y = v[1];
            spherePoints[i][j].z = v[2];
        }
    }

    // draw quads using generated vertices
    glBegin(GL_QUADS);

    for (unsigned int i = 0; i < pointsPerRow - 1; ++i)
    {
        for (unsigned int j = 0; j < pointsPerRow - 1; ++j)
        {
            // draw quad using 4 points from 2 rows
            glVertex3f(spherePoints[i][j].x, spherePoints[i][j].y, spherePoints[i][j].z);
            glVertex3f(spherePoints[i][j + 1].x, spherePoints[i][j + 1].y, spherePoints[i][j + 1].z);
            glVertex3f(spherePoints[i + 1][j + 1].x, spherePoints[i + 1][j + 1].y, spherePoints[i + 1][j + 1].z);
            glVertex3f(spherePoints[i + 1][j].x, spherePoints[i + 1][j].y, spherePoints[i + 1][j].z);
        }
    }
    glEnd();
}

void drawPlacedPartialSphere()
{
    glPushMatrix();
    {
        glTranslated(Sphere_transX, Sphere_transY, Sphere_transZ);
        glScaled(SPHERE_SCALE, SPHERE_SCALE, SPHERE_SCALE);
        drawPartialSphere(Sphere_subdivisions);
    }
    glPopMatrix();
}

void drawSphere()
{
    GLdouble sphereAngle = 90.0;
    bool colorAlternate = true;

    glPushMatrix();

    // Draw sides of the sphere
    for (int i = 0; i < 4; i++)
    {
        if (colorAlternate)
        {
            glColor3f(0, 0, 1);
            colorAlternate = !colorAlternate;
        }
        else
        {
            glColor3f(0, 1, 0);
            colorAlternate = !colorAlternate;
        }
        // glRotated(sphereAngle / 2, 1, 0, 0);
        // glRotated(sphereAngle * i, 0, 0, 1);
        drawPlacedPartialSphere();
        glRotated(sphereAngle, 0, 1, 0);
    }

    glPopMatrix();

    // Draw top and bottom of the sphere

    // Draw top
    glPushMatrix();
    {
        glColor3f(1, 0, 0);
        // glRotated(sphereAngle / 2, 1, 0, 0);
        glRotated(sphereAngle, 0, 0, 1);
        drawPlacedPartialSphere();

        // Draw bottom

        glColor3f(1, 0, 0);
        // glRotated(sphereAngle / 2, 1, 0, 0);
        glRotated(2 * sphereAngle, 0, 0, 1);
        drawPlacedPartialSphere();
    }
    glPopMatrix();
}

// Camera control

// Bonus task

void moveWithoutChangingReference(bool isUp)
{
    struct point temp_lookAt;

    temp_lookAt.x = pos.x + l.x;
    temp_lookAt.y = pos.y + l.y;
    temp_lookAt.z = pos.z + l.z;

    if (isUp)
    {
        pos.x += u.x * CAM_MOVE * 0.01;
        pos.y += u.y * CAM_MOVE * 0.01;
        pos.z += u.z * CAM_MOVE * 0.01;
    }
    else
    {
        pos.x -= u.x * CAM_MOVE * 0.01;
        pos.y -= u.y * CAM_MOVE * 0.01;
        pos.z -= u.z * CAM_MOVE * 0.01;
    }

    l.x = temp_lookAt.x - pos.x;
    l.y = temp_lookAt.y - pos.y;
    l.z = temp_lookAt.z - pos.z;
}

void keyboardListener(unsigned char key, int x, int y)
{
    // Rotation
    struct point l_perpendicular, r_perpendicular, u_perpendicular, rotatationAxis;

    switch (key)
    {
    // Rotate left/ Look left
    case '1':

        rotatationAxis = normalize(u);

        l_perpendicular = crossProduct(rotatationAxis, l);

        l.x = l.x * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.x * sin(DEG2RAD(CAM_ROTATE));
        l.y = l.y * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.y * sin(DEG2RAD(CAM_ROTATE));
        l.z = l.z * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.z * sin(DEG2RAD(CAM_ROTATE));

        r_perpendicular = crossProduct(rotatationAxis, r);

        r.x = r.x * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.x * sin(DEG2RAD(CAM_ROTATE));
        r.y = r.y * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.y * sin(DEG2RAD(CAM_ROTATE));
        r.z = r.z * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.z * sin(DEG2RAD(CAM_ROTATE));

        break;

    // Rotate right/ Look right
    case '2':

        rotatationAxis = normalize(u);
        l_perpendicular = crossProduct(rotatationAxis, l);

        l.x = l.x * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.x * sin(DEG2RAD(-CAM_ROTATE));
        l.y = l.y * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.y * sin(DEG2RAD(-CAM_ROTATE));
        l.z = l.z * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.z * sin(DEG2RAD(-CAM_ROTATE));

        r_perpendicular = crossProduct(rotatationAxis, r);

        r.x = r.x * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.x * sin(DEG2RAD(-CAM_ROTATE));
        r.y = r.y * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.y * sin(DEG2RAD(-CAM_ROTATE));
        r.z = r.z * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.z * sin(DEG2RAD(-CAM_ROTATE));

        break;

    // Look up
    case '3':

        rotatationAxis = normalize(r);
        l_perpendicular = crossProduct(rotatationAxis, l);

        l.x = l.x * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.x * sin(DEG2RAD(CAM_ROTATE));
        l.y = l.y * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.y * sin(DEG2RAD(CAM_ROTATE));
        l.z = l.z * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.z * sin(DEG2RAD(CAM_ROTATE));

        u_perpendicular = crossProduct(rotatationAxis, u);

        u.x = u.x * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.x * sin(DEG2RAD(CAM_ROTATE));
        u.y = u.y * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.y * sin(DEG2RAD(CAM_ROTATE));
        u.z = u.z * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.z * sin(DEG2RAD(CAM_ROTATE));

        break;

    // Look down
    case '4':

        rotatationAxis = normalize(r);
        l_perpendicular = crossProduct(rotatationAxis, l);

        l.x = l.x * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.x * sin(DEG2RAD(-CAM_ROTATE));
        l.y = l.y * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.y * sin(DEG2RAD(-CAM_ROTATE));
        l.z = l.z * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.z * sin(DEG2RAD(-CAM_ROTATE));

        u_perpendicular = crossProduct(rotatationAxis, u);

        u.x = u.x * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.x * sin(DEG2RAD(-CAM_ROTATE));
        u.y = u.y * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.y * sin(DEG2RAD(-CAM_ROTATE));
        u.z = u.z * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.z * sin(DEG2RAD(-CAM_ROTATE));

        break;

    // Roll clockwise
    case '5':

        rotatationAxis = normalize(l);
        r_perpendicular = crossProduct(rotatationAxis, r);

        r.x = r.x * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.x * sin(DEG2RAD(CAM_ROTATE));
        r.y = r.y * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.y * sin(DEG2RAD(CAM_ROTATE));
        r.z = r.z * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.z * sin(DEG2RAD(CAM_ROTATE));

        u_perpendicular = crossProduct(rotatationAxis, u);

        u.x = u.x * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.x * sin(DEG2RAD(CAM_ROTATE));
        u.y = u.y * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.y * sin(DEG2RAD(CAM_ROTATE));
        u.z = u.z * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.z * sin(DEG2RAD(CAM_ROTATE));

        break;

    // Roll anti-clockwise
    case '6':

        rotatationAxis = normalize(l);
        r_perpendicular = crossProduct(rotatationAxis, r);

        r.x = r.x * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.x * sin(DEG2RAD(-CAM_ROTATE));
        r.y = r.y * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.y * sin(DEG2RAD(-CAM_ROTATE));
        r.z = r.z * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.z * sin(DEG2RAD(-CAM_ROTATE));

        u_perpendicular = crossProduct(rotatationAxis, u);

        u.x = u.x * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.x * sin(DEG2RAD(-CAM_ROTATE));
        u.y = u.y * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.y * sin(DEG2RAD(-CAM_ROTATE));
        u.z = u.z * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.z * sin(DEG2RAD(-CAM_ROTATE));

        break;

    // Move up without changing reference
    // case 'w':
    case 'w':
        moveWithoutChangingReference(true);

        break;

    // Move down without changing reference
    case 's':
        moveWithoutChangingReference(false);

        break;

    // Rotate clockwise around self axis
    case 'a':
          
          rotation_angle -= rotation;
          break;

    // Rotate anti-clockwise around self axis
    case 'd':
          rotation_angle += rotation;
          break;

    case ',':
        transition_Octahedron_To_Sphere();
        break;

    case '.':
        transition_Sphere_To_Octahedron();
        break;

    default:
        printf("We don't know what you pressed\n");
        break;
    }
}

void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    // Translation

    // Move forward
    case GLUT_KEY_UP:
        pos.x += l.x * CAM_MOVE;
        pos.y += l.y * CAM_MOVE;
        pos.z += l.z * CAM_MOVE;
        break;

    // Move backward
    case GLUT_KEY_DOWN:
        pos.x -= l.x * CAM_MOVE;
        pos.y -= l.y * CAM_MOVE;
        pos.z -= l.z * CAM_MOVE;
        break;

    // Move right
    case GLUT_KEY_RIGHT:
        pos.x += r.x * CAM_MOVE;
        pos.y += r.y * CAM_MOVE;
        pos.z += r.z * CAM_MOVE;
        break;

    // Move left
    case GLUT_KEY_LEFT:
        pos.x -= r.x * CAM_MOVE;
        pos.y -= r.y * CAM_MOVE;
        pos.z -= r.z * CAM_MOVE;
        break;

    // Move up
    case GLUT_KEY_PAGE_UP:
        pos.x += u.x * CAM_MOVE;
        pos.y += u.y * CAM_MOVE;
        pos.z += u.z * CAM_MOVE;
        break;

    // Move down
    case GLUT_KEY_PAGE_DOWN:
        pos.x -= u.x * CAM_MOVE;
        pos.y -= u.y * CAM_MOVE;
        pos.z -= u.z * CAM_MOVE;
        break;

    default:
        break;
    }
}

void display()
{

    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pos.x, pos.y, pos.z,
              pos.x + l.x, pos.y + l.y, pos.z + l.z,
              u.x, u.y, u.z);

    glMatrixMode(GL_MODELVIEW);
    drawAxes(LENGTH * 6);

    // Rotate around self axis
    glRotated(rotation_angle, 0, 0, 1);

    if (isOctahedronOn)
    {
        glPushMatrix();
        {
            drawOctahedron();
        }
        glPopMatrix();
    }

    if (isCylinderOn)
    {
        glPushMatrix();
        {
            drawCylinderSet();
        }
        glPopMatrix();
    }

    if (isSphereOn)
    {
        glPushMatrix();
        {
            drawSphere();
        }
        glPopMatrix();
    }

    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitWindowSize(650, 650);     // Set the window's initial width & height
    glutInitWindowPosition(750, 250); // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Offline 1");

    init();

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}