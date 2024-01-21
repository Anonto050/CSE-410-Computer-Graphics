#ifndef _constants_h_
#define _constants_h_

// Includes
#include <bits/stdc++.h>
#ifdef __linux__
#include <GL/glut.h>
#elif WIN32
#include <windows.h>
#include <glut.h>
#endif

using namespace std;

// Constants

#define pi (2 * acos(0.0))
#define DEG2RAD(deg) (deg * pi / 180)
#define CAM_MOVE 5.0
#define CAM_ROTATE 10.0

#define LENGTH 50

#define TRANSISTION_STEPS 16

#define SPACEBAR 32


// Magic cube constants

GLfloat rotation_angle = 0;
const GLfloat rotation = 10;


// Cylinder constants
const GLfloat MAX_CYLINDER_RADIUS = LENGTH * 0.577;
const GLfloat MAX_CYLINDER_HEIGHT = sqrt(2) * LENGTH;
GLfloat CYLINDER_RADIUS = MAX_CYLINDER_RADIUS;
GLfloat CYLINDER_HEIGHT = MAX_CYLINDER_HEIGHT;
const GLfloat CYLINDER_ANGLE = M_PI * 2;
const GLfloat CYLINDER_MAX_Translation = (1/sqrt(2)) * LENGTH;
GLfloat CYLINDER_TRANSLATION = CYLINDER_MAX_Translation;
GLfloat CYLINDER_SCALE = 0;

// Octahedron constants
const GLfloat Triangle_Centre_X = 0.3333*LENGTH;
const GLfloat Triangle_Centre_Y = 0.3333*LENGTH;
const GLfloat Triangle_Centre_Z = 0.3333*LENGTH;

GLfloat Basic_Triangle_Centre_X = 0;
GLfloat Basic_Triangle_Centre_Y = 0;
GLfloat Basic_Triangle_Centre_Z = 0;

GLfloat TRIANGLE_SCALE = 1;


// Sphere constants
GLfloat Sphere_transX = LENGTH;
GLfloat Sphere_transY = 0;
GLfloat Sphere_transZ = 0;
GLfloat Sphere_subdivisions = 5;

GLfloat SPHERE_SCALE = 0;



// Rolling ball constants

// Ball constants
const GLfloat ball_radius = 15;
const GLfloat ball_scale = 0.5;
const GLfloat ball_slice = 15;
const GLfloat ball_stack = 15;
const GLfloat ball_rotate = 5;
const GLfloat ball_speed = 0.05;

// Arrow constants
const GLfloat arrow_length = 40;
const GLfloat arrow_width = 5;
GLfloat arrow_angle = 0;
const GLfloat arrow_rotate = 14;

#endif