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
bool isSphereOn = true;

bool simulation_mode = false;

struct point
{
    double x, y, z;
};

// Here we will be storing the camera position, lookAt position and the up vector.
struct point pos, u, r, l;

struct point ball_center, ball_direction;

struct point points[100][100];

struct point points_lower_hemisphere[100][100];

bool newSphere = true;

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
    pos.z = 30.0;

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

    // Initialize ball center
    ball_center.x = 0;
    ball_center.y = 0;
    ball_center.z = ball_radius;

    // Initialize ball direction
    ball_direction.x = arrow_length;
    ball_direction.y = 0;
    ball_direction.z = ball_radius;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(80, 1, 1, 1000.0);
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

// Sphere Drawing

void drawSphere(double radius, int slices, int stacks)
{
    int i, j;
    double h, r;

    if (newSphere)
    {
        // generate points
        for (i = 0; i <= stacks; i++)
        {
            h = radius * sin(((double)i / (double)stacks) * (pi / 2));
            r = radius * cos(((double)i / (double)stacks) * (pi / 2));
            for (j = 0; j <= slices; j++)
            {
                points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
                points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
                points[i][j].z = h;

                // Generate points for the lower hemisphere
                points_lower_hemisphere[i][j].x = points[i][j].x;
                points_lower_hemisphere[i][j].y = points[i][j].y;
                points_lower_hemisphere[i][j].z = points[i][j].z;
            }
        }
    }

    // draw quads using generated points
    for (i = 0; i < stacks; i++)
    {

        bool color = true;

        for (j = 0; j < slices; j++)
        {

            glBegin(GL_QUADS);
            {

                // upper hemisphere

                if (color)
                {
                    glColor3f(1, 0, 0);
                }
                else
                {
                    glColor3f(0, 1, 0);
                }
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);

                // lower hemisphere

                if (!color)
                {
                    glColor3f(1, 0, 0);
                    color = !color;
                }
                else
                {
                    glColor3f(0, 1, 0);
                    color = !color;
                }

                if (newSphere)
                {
                    glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                    glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                    glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                    glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
                }
                else
                {
                    glVertex3f(points_lower_hemisphere[i][j].x, points_lower_hemisphere[i][j].y, -points_lower_hemisphere[i][j].z);
                    glVertex3f(points_lower_hemisphere[i][j + 1].x, points_lower_hemisphere[i][j + 1].y, -points_lower_hemisphere[i][j + 1].z);
                    glVertex3f(points_lower_hemisphere[i + 1][j + 1].x, points_lower_hemisphere[i + 1][j + 1].y, -points_lower_hemisphere[i + 1][j + 1].z);
                    glVertex3f(points_lower_hemisphere[i + 1][j].x, points_lower_hemisphere[i + 1][j].y, -points_lower_hemisphere[i + 1][j].z);
                }
            }
            glEnd();
        }
    }

    newSphere = false;
}

void goForward()
{
    struct point rotation_axis, temp_point;

    rotation_axis.x = ball_direction.y * (-1);
    rotation_axis.y = ball_direction.x;
    // rotation_axis.z = ball_direction.z;
    rotation_axis.z = 0;

    rotation_axis = normalize(rotation_axis);

    // Rotate every point of the sphere around the rotation axis
    for (int i = 0; i <= ball_stack; i++)
    {
        for (int j = 0; j <= ball_slice; j++)
        {
            temp_point = crossProduct(rotation_axis, points[i][j]);

            points[i][j].x = points[i][j].x * cos(DEG2RAD(ball_rotate)) + temp_point.x * sin(DEG2RAD(ball_rotate));
            points[i][j].y = points[i][j].y * cos(DEG2RAD(ball_rotate)) + temp_point.y * sin(DEG2RAD(ball_rotate));
            points[i][j].z = points[i][j].z * cos(DEG2RAD(ball_rotate)) + temp_point.z * sin(DEG2RAD(ball_rotate));

            // Rotate for the lower hemisphere
            temp_point = crossProduct(points_lower_hemisphere[i][j], rotation_axis);

            points_lower_hemisphere[i][j].x = points_lower_hemisphere[i][j].x * cos(DEG2RAD(ball_rotate)) + temp_point.x * sin(DEG2RAD(ball_rotate));
            points_lower_hemisphere[i][j].y = points_lower_hemisphere[i][j].y * cos(DEG2RAD(ball_rotate)) + temp_point.y * sin(DEG2RAD(ball_rotate));
            points_lower_hemisphere[i][j].z = points_lower_hemisphere[i][j].z * cos(DEG2RAD(ball_rotate)) + temp_point.z * sin(DEG2RAD(ball_rotate));
        }
    }
}

// Rotate counterclockwise to go backwards

void goBackwards()
{
    struct point rotation_axis, temp_point;

    rotation_axis.x = ball_direction.y;
    rotation_axis.y = ball_direction.x * (-1);
    // rotation_axis.z = ball_direction.z;
    rotation_axis.z = 0;

    rotation_axis = normalize(rotation_axis);

    // Rotate every point of the sphere around the rotation axis
    for (int i = 0; i <= ball_stack; i++)
    {
        for (int j = 0; j <= ball_slice; j++)
        {
            temp_point = crossProduct(rotation_axis, points[i][j]);

            points[i][j].x = points[i][j].x * cos(DEG2RAD(ball_rotate)) + temp_point.x * sin(DEG2RAD(ball_rotate));
            points[i][j].y = points[i][j].y * cos(DEG2RAD(ball_rotate)) + temp_point.y * sin(DEG2RAD(ball_rotate));
            points[i][j].z = points[i][j].z * cos(DEG2RAD(ball_rotate)) + temp_point.z * sin(DEG2RAD(ball_rotate));

            // Rotate for the lower hemisphere
            temp_point = crossProduct(points_lower_hemisphere[i][j], rotation_axis);

            points_lower_hemisphere[i][j].x = points_lower_hemisphere[i][j].x * cos(DEG2RAD(ball_rotate)) + temp_point.x * sin(DEG2RAD(ball_rotate));
            points_lower_hemisphere[i][j].y = points_lower_hemisphere[i][j].y * cos(DEG2RAD(ball_rotate)) + temp_point.y * sin(DEG2RAD(ball_rotate));
            points_lower_hemisphere[i][j].z = points_lower_hemisphere[i][j].z * cos(DEG2RAD(ball_rotate)) + temp_point.z * sin(DEG2RAD(ball_rotate));
        }
    }
}



// Draw arrow

void drawArrow(struct point center, struct point direction)
{
    // Draw arrow line
    glBegin(GL_QUADS);
    {
        glColor3f(0, 0, 1);
        glVertex3f(center.x, center.y, center.z);
        glVertex3f(center.x + direction.x, center.y + direction.y, direction.z);
        glVertex3f(center.x + direction.x + arrow_width, center.y + direction.y + arrow_width, direction.z);
        glVertex3f(center.x + arrow_width, center.y + arrow_width, center.z);
    }
    glEnd();

    // Draw arrow head
    glBegin(GL_TRIANGLES);
    {
        glColor3f(0, 0, 1);
        glVertex3f(center.x + direction.x, center.y + direction.y, direction.z);
        glVertex3f(center.x + direction.x + arrow_width, center.y + direction.y + arrow_width, direction.z);
        glVertex3f(center.x + direction.x - arrow_width, center.y + direction.y + arrow_width, direction.z);
    }
    glEnd();
}

void drawWalls()
{

    // Draw four walls around the checkerboard

    // Wall 1
    glPushMatrix();

    glBegin(GL_QUADS);
    {
        glColor3f(1, 0, 0);
        glVertex3f(LENGTH * 3, -LENGTH * 3, 0);
        glVertex3f(-LENGTH * 3, -LENGTH * 3, 0);
        glVertex3f(-LENGTH * 3, -LENGTH * 3, 15);
        glVertex3f(LENGTH * 3, -LENGTH * 3, 15);
    }
    glEnd();
    glPopMatrix();

    // Wall 2
    glPushMatrix();
    glBegin(GL_QUADS);
    {
        glColor3f(1, 0, 0);
        glVertex3f(-LENGTH * 3, LENGTH * 3, 0);
        glVertex3f(-LENGTH * 3, -LENGTH * 3, 0);
        glVertex3f(-LENGTH * 3, -LENGTH * 3, 15);
        glVertex3f(-LENGTH * 3, LENGTH * 3, 15);
    }
    glEnd();
    glPopMatrix();

    // Wall 3
    glPushMatrix();
    glBegin(GL_QUADS);
    {
       glColor3f(1, 0, 0);
        glVertex3f(LENGTH * 3, LENGTH * 3, 0);
        glVertex3f(LENGTH * 3, -LENGTH * 3, 0);
        glVertex3f(LENGTH * 3, -LENGTH * 3, 15);
        glVertex3f(LENGTH * 3, LENGTH * 3, 15);
    }
    glEnd();
    glPopMatrix();

    // Wall 4
    glPushMatrix();
    glBegin(GL_QUADS);
    {
        glColor3f(1, 0, 0);
        glVertex3f(LENGTH * 3, LENGTH * 3, 0);
        glVertex3f(-LENGTH * 3, LENGTH * 3, 0);
        glVertex3f(-LENGTH * 3, LENGTH * 3, 15);
        glVertex3f(LENGTH * 3, LENGTH * 3, 15);
    }
    glEnd();
    glPopMatrix();
}

// Draw checkerboard

void drawSquare(int tileLength)
{
    glBegin(GL_QUADS);
    {
        glVertex3f(0, 0, 0);
        glVertex3f(tileLength, 0, 0);
        glVertex3f(tileLength, tileLength, 0);
        glVertex3f(0, tileLength, 0);
    }
    glEnd();
}

void drawCheckerBoard()
{
    int i, j;
    int tileLength = 20;
    int tileCount = 30;

    for (i = 0; i < tileCount; i++)
    {
        for (j = 0; j < tileCount; j++)
        {
            if ((i + j) % 2 == 0)
            {
                glColor3f(0, 0, 0);
            }
            else
            {
                glColor3f(1, 1, 1);
            }

            glPushMatrix();
            {
                glTranslatef(i * tileLength, j * tileLength, 0);
                drawSquare(tileLength);
            }
            glPopMatrix();

            glPushMatrix();
            {
                glTranslatef(-i * tileLength, j * tileLength, 0);
                drawSquare(tileLength);
            }
            glPopMatrix();

            glPushMatrix();
            {
                glTranslatef(i * tileLength, -j * tileLength, 0);
                drawSquare(tileLength);
            }
            glPopMatrix();

            glPushMatrix();
            {
                glTranslatef(-i * tileLength, -j * tileLength, 0);
                drawSquare(tileLength);
            }
            glPopMatrix();
        }
    }
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

        // pos.z += CAM_MOVE * 0.01;

        // u = crossProduct(r, l);
    }
    else
    {
        pos.x -= u.x * CAM_MOVE * 0.01;
        pos.y -= u.y * CAM_MOVE * 0.01;
        pos.z -= u.z * CAM_MOVE * 0.01;

        // pos.z -= CAM_MOVE * 0.01;

        // u = crossProduct(r, l);
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

    case 'j':

        arrow_angle += arrow_rotate;
        ball_direction.x = arrow_length * cos(DEG2RAD(arrow_angle));
        ball_direction.y = arrow_length * sin(DEG2RAD(arrow_angle));

        break;

    case 'l':
        arrow_angle -= arrow_rotate;
        ball_direction.x = arrow_length * cos(DEG2RAD(arrow_angle));
        ball_direction.y = arrow_length * sin(DEG2RAD(arrow_angle));

        break;

    case 'i':

        ball_center.x += ball_speed * ball_direction.x;
        ball_center.y += ball_speed * ball_direction.y;

        goForward();
        break;

    case 'k':

        ball_center.x -= ball_speed * ball_direction.x;
        ball_center.y -= ball_speed * ball_direction.y;

        goBackwards();
        break;

    // Change modes between simulation and manual control by pressing spacebar
    case SPACEBAR:
        simulation_mode = !simulation_mode;
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

    // Draw axes
    drawAxes(LENGTH * 6);

    if(simulation_mode)
    {
        ball_center.x += ball_speed * ball_direction.x;
        ball_center.y += ball_speed * ball_direction.y;

        goForward();
    }

    // Draw walls
    drawWalls();

    // Draw arrow
    drawArrow(ball_center, ball_direction);

    // Draw checkerboard
    glPushMatrix();
    {
        drawCheckerBoard();
    }
    glPopMatrix();

    if (isSphereOn)
    {
        glPushMatrix();
        {
            glTranslated(ball_center.x, ball_center.y, ball_center.z);
            glScaled(ball_scale, ball_scale, ball_scale);
            drawSphere(ball_radius, ball_slice, ball_stack);
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

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutIdleFunc(idle);

    init();

    glutMainLoop();
    return 0;
}