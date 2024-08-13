// #include<bits/stdc++.h>

// #ifdef __linux__
//     #include<GL/glut.h>
// #elif WIN32
//     #include <windows.h>
//     #include <glut.h>
// #endif


// using namespace std;

// void init(){
//     printf("Do your initialization here\n");
//     glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    

// }

// // a openGL integer
// GLint counter = 0;

// void keyboardListener(unsigned char key,int x, int y){
//     switch (key)
//     {
//     case 'K':
//         /* code */
//         printf("K pressed\n");
//         break;
    
//     default:
//         printf("We don't know what you pressed\n");
//         break;
//     }
// }
// void drawSquare(double a){
//     glBegin(GL_QUADS);{
//         glVertex3f( a, a,0);
//         glVertex3f( a,-a,0);
//         glVertex3f(-a,-a,0);
//         glVertex3f(-a, a,0);
//     }glEnd();

// }

// double radius = 0.3;

// void drawCircle(){
    
//     double prev_x = radius;
//     double prev_y = 0;
//     double theta = 0;

//     while (theta<=2*3.14){
//         prev_x = radius*cos(theta);
//         prev_y = radius*sin(theta);

//         glBegin(GL_LINES);{
//             glVertex3f(0, 0, 0);
//             glVertex3f(prev_x, prev_y, 0);
//         }glEnd();

//         theta += 0.01;
//     }
// }

// void display(){
//     printf("Display function called for %d times\n", counter);
    
//     // glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set background color to black and opaque
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     glBegin(GL_LINES);{
//         glColor3f(0.0f, 0.0f, 1.0f); // Green
//         glVertex2f(-1.0f, 0.0f);
//         glVertex2f(1.0f, 0.0f);
//         glVertex2f(0.0f, -1.0f);
//         glVertex2f(0.0f, 1.0f);
       
//     }glEnd();


//     // for(int i = 0; i < 8; i++){
//         glPushMatrix();

//         glRotatef((counter)%360 , 0, 0, 1);
//         glTranslatef(0.5, 0.5, 0);

//         // glRotatef((counter)%360 , 0, 0, 1);
        
//         // glColor3f(1.0f, 0.0f, 0.0f); // Red
//         drawSquare(0.1);
//         glPopMatrix();
//     // }
     
//     //  glColor3f(1.0f, 0.0f, 0.0f); // Red
//     //  drawCircle();


//     // glFlush();
//     glutSwapBuffers();

// }

// void animate(int value){
//    counter++;
//    glutTimerFunc(10, animate, 0);
// }

// void idle(){

//     // counter++;
//     glutPostRedisplay();
// }

// int main(int argc,char** argv){
//     glutInit(&argc,argv);
//     glutInitWindowSize(720, 450);   // Set the window's initial width & height
//     glutInitWindowPosition(750, 250); // Position the window's initial top-left corner
//     glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//     glutCreateWindow("Test");
//     glutDisplayFunc(display);
//     glutKeyboardFunc(keyboardListener);
//     glutIdleFunc(idle);
//     init();

//     glutTimerFunc(100, animate, 0);

//     glutMainLoop();
//     return 0;

// }


#include <GL/glut.h>  
#include <math.h>    
#define PI 3.14159265f
 
// Global variables
char title[] = "Bouncing Ball (2D)";  // Windowed mode's title
int windowWidth  = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX   = 50;      // Windowed mode's top-left corner x
int windowPosY   = 50;      // Windowed mode's top-left corner y
 
GLfloat ballRadius = 0.5f;   // Radius of the bouncing ball
GLfloat ballX = 0.0f;         // Ball's center (x, y) position
GLfloat ballY = 0.0f;
GLfloat ballXMax, ballXMin, ballYMax, ballYMin; // Ball's center (x, y) bounds
GLfloat xSpeed = 0.02f;      // Ball's speed in x and y directions
GLfloat ySpeed = 0.007f;
int refreshMillis = 30;      // Refresh period in milliseconds
 
// Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;
 
/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
}
 
/* Callback handler for window re-paint event */
void display() {
   glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
   glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
   glLoadIdentity();              // Reset model-view matrix
 
   glTranslatef(ballX, ballY, 0.0f);  // Translate to (xPos, yPos)
   // Use triangular segments to form a circle
   glBegin(GL_TRIANGLE_FAN);
      glColor3f(0.0f, 0.0f, 1.0f);  // Blue
      glVertex2f(0.0f, 0.0f);       // Center of circle
      int numSegments = 100;
      GLfloat angle;
      for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
         angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
         glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
      }
   glEnd();
 
   glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)
 
   // Animation Control - compute the location for the next refresh
   ballX += xSpeed;
   ballY += ySpeed;
   // Check if the ball exceeds the edges
   if (ballX > ballXMax) {
      ballX = ballXMax;
      xSpeed = -xSpeed;
   } else if (ballX < ballXMin) {
      ballX = ballXMin;
      xSpeed = -xSpeed;
   }
   if (ballY > ballYMax) {
      ballY = ballYMax;
      ySpeed = -ySpeed;
   } else if (ballY < ballYMin) {
      ballY = ballYMin;
      ySpeed = -ySpeed;
   }
}
 
/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset the projection matrix
   if (width >= height) {
      clipAreaXLeft   = -1.0 * aspect;
      clipAreaXRight  = 1.0 * aspect;
      clipAreaYBottom = -1.0;
      clipAreaYTop    = 1.0;
   } else {
      clipAreaXLeft   = -1.0;
      clipAreaXRight  = 1.0;
      clipAreaYBottom = -1.0 / aspect;
      clipAreaYTop    = 1.0 / aspect;
   }
   gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
   ballXMin = clipAreaXLeft + ballRadius;
   ballXMax = clipAreaXRight - ballRadius;
   ballYMin = clipAreaYBottom + ballRadius;
   ballYMax = clipAreaYTop - ballRadius;
}
 
/* Called back when the timer expired */
void Timer(int value) {
   glutPostRedisplay();    // Post a paint request to activate display()
   glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
}
 
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
   glutCreateWindow(title);      // Create window with given title
   glutDisplayFunc(display);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   glutTimerFunc(0, Timer, 0);   // First timer call immediately
   initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 0;
}