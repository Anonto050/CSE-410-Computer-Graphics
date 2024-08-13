#include <string>

#include "classes.h"

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define VIEW_ANGLE 90

#define DEG2RAD(deg) (deg * PI / 180)
#define CAM_MOVE 5.0
#define CAM_ROTATE 10.0

extern vector<Object *> objects;
extern vector<PointLight *> point_lights;
extern vector<SpotLight *> spot_lights;

extern int recursion_level;

int pixel_width;
int pixel_height;

int drawaxes;
int drawgrid;

int capture_counter;

Vector3D pos, u, r, l;

void loadData()
{
    ifstream file;

    file.open("scene2.txt");

    if (!file.is_open())
    {
        cout << "Error: File not found" << endl;
        return;
    }

    int shine;

    double red, green, blue;
    double ambience, diffuse, specular, reflection;

    int num_objects;

    Object *obj;

    if (file.is_open())
    {

        file >> recursion_level;

        file >> pixel_width;

        pixel_height = pixel_width;

        file >> num_objects;

        for (int i = 0; i < num_objects; i++)
        {
            string type;
            file >> type;

            if (type == "sphere")
            {

                Vector3D center;
                double radius;

                file >> center >> radius;

                obj = new Sphere(center, radius);
            }
            else if (type == "triangle")
            {

                Vector3D v1, v2, v3;

                file >> v1 >> v2 >> v3;

                obj = new Triangle(v1, v2, v3);
            }
            else if (type == "general")
            {

                double A, B, C, D, E, F, G, H, I, J;
                double length, width, height;
                Vector3D ref_point;

                file >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;

                file >> ref_point >> length >> width >> height;

                obj = new General(A, B, C, D, E, F, G, H, I, J, ref_point, height, width, length);
            }

            file >> red >> green >> blue;

            Color color(red, green, blue);

            obj->setColor(color);

            file >> ambience >> diffuse >> specular >> reflection >> shine;

            obj->setCoEfficients(ambience, diffuse, specular, reflection);

            obj->setShine(shine);

            objects.push_back(obj);
        }

        int num_point_lights;

        file >> num_point_lights;

        PointLight *point_light;

        for (int i = 0; i < num_point_lights; i++)
        {

            Vector3D position;

            file >> position;

            Color color;

            file >> color;

            point_light = new PointLight(position, color);

            point_lights.push_back(point_light);
        }

        int num_spot_lights;

        file >> num_spot_lights;

        SpotLight *spot_light;

        for (int i = 0; i < num_spot_lights; i++)
        {

            Vector3D position, direction;

            double angle;

            file >> position;

            Color color;

            file >> color;

            file >> direction;

            file >> angle;

            point_light = new PointLight(position, color);

            spot_light = new SpotLight(*point_light, direction, angle);

            spot_lights.push_back(spot_light);
        }

        obj = new Floor(20, 1000);

        obj->setShine(10);
        obj->setCoEfficients(0.4, 0.2, 0.2, 0.2);

        objects.push_back(obj);

        file.close();
    }
}

void memoryCleanUp()
{
    for (int i = 0; i < objects.size(); i++)
    {
        delete objects[i];
    }

    for (int i = 0; i < point_lights.size(); i++)
    {
        delete point_lights[i];
    }

    for (int i = 0; i < spot_lights.size(); i++)
    {
        delete spot_lights[i];
    }

    objects.clear();
    point_lights.clear();
    spot_lights.clear();

    return;
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

void drawGrid()
{
    int i;
    if (drawgrid == 1)
    {
        glColor3f(0.6, 0.6, 0.6); // grey
        glBegin(GL_LINES);
        {
            for (i = -20; i <= 20; i++)
            {

                if (i == 0)
                    continue; // SKIP the MAIN axes

                // lines parallel to Y-axis
                glVertex3f(i * 10, -200, 0);
                glVertex3f(i * 10, 200, 0);

                // lines parallel to X-axis
                glVertex3f(-200, i * 10, 0);
                glVertex3f(200, i * 10, 0);
            }
        }
        glEnd();
    }
}

void initBitmapImage(bitmap_image *image)
{
    for (int i = 0; i < pixel_height; i++)
    {
        for (int j = 0; j < pixel_width; j++)
        {
            image->set_pixel(i, j, 0, 0, 0);
        }
    }
}

Ray createRay(int i, int j)
{
    double plane_distance = ((double)SCREEN_WIDTH / 2.0) / tan(DEG2RAD(VIEW_ANGLE) / 2.0);

    Vector3D topleft = pos + l * plane_distance - r * ((double)SCREEN_WIDTH / 2.0) + u * ((double)SCREEN_HEIGHT / 2.0);

    double du = (double)SCREEN_WIDTH / (double)pixel_width;
    double dv = (double)SCREEN_HEIGHT / (double)pixel_height;

    topleft = topleft + r * (du / 2.0) - u * (dv / 2.0);

    Vector3D corner = topleft + r * (j * du) - u * (i * dv);

    Vector3D dir = corner - pos;

    Ray ray(pos, dir);
    ray.normalize();

    return ray;
}

void capture()
{
    bitmap_image *image;
    image = new bitmap_image(pixel_width, pixel_height);

    initBitmapImage(image);


    for (int i = 0; i < pixel_height; i++)
    {
        for (int j = 0; j < pixel_width; j++)
        {
            Ray ray = createRay(i, j);

            double t_min = INF;
            int nearest_object_index = -1;

            Color color(0, 0, 0);

            for (int k = 0; k < objects.size(); k++)
            {
                double t = objects[k]->intersect(ray, color, 0);

                if (t >= 0 && (t_min - t) > EPSILON)
                {
                    t_min = t;
                    nearest_object_index = k;
                }
            }

            if (abs(t_min - INF) > EPSILON)
            {
                objects[nearest_object_index]->intersect(ray, color, 1);

                // check if any color value is greater than 1, make it 1
                if (color.getR() > 1)
                    color.setR(1);

                if (color.getG() > 1)
                    color.setG(1);

                if (color.getB() > 1)
                    color.setB(1);

                image->set_pixel(j, i, 255 * color.getR(), 255 * color.getG(), 255 * color.getB());
            }
        }
    }

    image->save_image("output" + to_string(capture_counter) + ".bmp");

    delete image;

    return;
}

void keyboardListener(unsigned char key, int x, int y)
{
    // Rotation
    Vector3D l_perpendicular, r_perpendicular, u_perpendicular, rotatationAxis;

    switch (key)
    {

    // Capture
    case '0':
        capture_counter++;
        capture();
        break;

    // Rotate left/ Look left
    case '1':

        rotatationAxis = u.normalize();
        l_perpendicular = rotatationAxis.crossProduct(l);

        l.setX(l.getX() * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.getX() * sin(DEG2RAD(CAM_ROTATE)));
        l.setY(l.getY() * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.getY() * sin(DEG2RAD(CAM_ROTATE)));
        l.setZ(l.getZ() * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.getZ() * sin(DEG2RAD(CAM_ROTATE)));

        r_perpendicular = rotatationAxis.crossProduct(r);

        r.setX(r.getX() * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.getX() * sin(DEG2RAD(CAM_ROTATE)));
        r.setY(r.getY() * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.getY() * sin(DEG2RAD(CAM_ROTATE)));
        r.setZ(r.getZ() * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.getZ() * sin(DEG2RAD(CAM_ROTATE)));

        break;

    // Rotate right/ Look right
    case '2':

        rotatationAxis = u.normalize();
        l_perpendicular = rotatationAxis.crossProduct(l);

        l.setX(l.getX() * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.getX() * sin(DEG2RAD(-CAM_ROTATE)));
        l.setY(l.getY() * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.getY() * sin(DEG2RAD(-CAM_ROTATE)));
        l.setZ(l.getZ() * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.getZ() * sin(DEG2RAD(-CAM_ROTATE)));

        r_perpendicular = rotatationAxis.crossProduct(r);

        r.setX(r.getX() * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.getX() * sin(DEG2RAD(-CAM_ROTATE)));
        r.setY(r.getY() * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.getY() * sin(DEG2RAD(-CAM_ROTATE)));
        r.setZ(r.getZ() * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.getZ() * sin(DEG2RAD(-CAM_ROTATE)));

        break;

    // Look up
    case '3':

        rotatationAxis = r.normalize();
        l_perpendicular = rotatationAxis.crossProduct(l);

        l.setX(l.getX() * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.getX() * sin(DEG2RAD(CAM_ROTATE)));
        l.setY(l.getY() * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.getY() * sin(DEG2RAD(CAM_ROTATE)));
        l.setZ(l.getZ() * cos(DEG2RAD(CAM_ROTATE)) + l_perpendicular.getZ() * sin(DEG2RAD(CAM_ROTATE)));

        u_perpendicular = rotatationAxis.crossProduct(u);

        u.setX(u.getX() * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.getX() * sin(DEG2RAD(CAM_ROTATE)));
        u.setY(u.getY() * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.getY() * sin(DEG2RAD(CAM_ROTATE)));
        u.setZ(u.getZ() * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.getZ() * sin(DEG2RAD(CAM_ROTATE)));

        break;

    // Look down
    case '4':

        rotatationAxis = r.normalize();
        l_perpendicular = rotatationAxis.crossProduct(l);

        l.setX(l.getX() * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.getX() * sin(DEG2RAD(-CAM_ROTATE)));
        l.setY(l.getY() * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.getY() * sin(DEG2RAD(-CAM_ROTATE)));
        l.setZ(l.getZ() * cos(DEG2RAD(-CAM_ROTATE)) + l_perpendicular.getZ() * sin(DEG2RAD(-CAM_ROTATE)));

        u_perpendicular = rotatationAxis.crossProduct(u);

        u.setX(u.getX() * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.getX() * sin(DEG2RAD(-CAM_ROTATE)));
        u.setY(u.getY() * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.getY() * sin(DEG2RAD(-CAM_ROTATE)));
        u.setZ(u.getZ() * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.getZ() * sin(DEG2RAD(-CAM_ROTATE)));

        break;

    // Roll clockwise
    case '5':

        rotatationAxis = l.normalize();
        r_perpendicular = rotatationAxis.crossProduct(r);

        r.setX(r.getX() * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.getX() * sin(DEG2RAD(CAM_ROTATE)));
        r.setY(r.getY() * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.getY() * sin(DEG2RAD(CAM_ROTATE)));
        r.setZ(r.getZ() * cos(DEG2RAD(CAM_ROTATE)) + r_perpendicular.getZ() * sin(DEG2RAD(CAM_ROTATE)));

        u_perpendicular = rotatationAxis.crossProduct(u);

        u.setX(u.getX() * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.getX() * sin(DEG2RAD(CAM_ROTATE)));
        u.setY(u.getY() * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.getY() * sin(DEG2RAD(CAM_ROTATE)));
        u.setZ(u.getZ() * cos(DEG2RAD(CAM_ROTATE)) + u_perpendicular.getZ() * sin(DEG2RAD(CAM_ROTATE)));

        break;

    // Roll anti-clockwise
    case '6':

        rotatationAxis = l.normalize();
        r_perpendicular = rotatationAxis.crossProduct(r);

        r.setX(r.getX() * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.getX() * sin(DEG2RAD(-CAM_ROTATE)));
        r.setY(r.getY() * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.getY() * sin(DEG2RAD(-CAM_ROTATE)));
        r.setZ(r.getZ() * cos(DEG2RAD(-CAM_ROTATE)) + r_perpendicular.getZ() * sin(DEG2RAD(-CAM_ROTATE)));

        u_perpendicular = rotatationAxis.crossProduct(u);

        u.setX(u.getX() * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.getX() * sin(DEG2RAD(-CAM_ROTATE)));
        u.setY(u.getY() * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.getY() * sin(DEG2RAD(-CAM_ROTATE)));
        u.setZ(u.getZ() * cos(DEG2RAD(-CAM_ROTATE)) + u_perpendicular.getZ() * sin(DEG2RAD(-CAM_ROTATE)));

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
        pos = pos + l * CAM_MOVE;

        break;

    // Move backward
    case GLUT_KEY_DOWN:
        pos = pos - l * CAM_MOVE;

        break;

    // Move right
    case GLUT_KEY_RIGHT:
        pos = pos + r * CAM_MOVE;

        break;

    // Move left
    case GLUT_KEY_LEFT:
        pos = pos - r * CAM_MOVE;

        break;

    // Move up
    case GLUT_KEY_PAGE_UP:
        pos = pos + u * CAM_MOVE;

        break;

    // Move down
    case GLUT_KEY_PAGE_DOWN:
        pos = pos - u * CAM_MOVE;

        break;

    default:
        break;
    }
}

void mouseListener(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            drawaxes = 1 - drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            drawgrid = 1 - drawgrid;
        }
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0); // color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(pos.getX(), pos.getY(), pos.getZ(),
              pos.getX() + l.getX(), pos.getY() + l.getY(), pos.getZ() + l.getZ(),
              u.getX(), u.getY(), u.getZ());

    glMatrixMode(GL_MODELVIEW);
    drawAxes(100);
    drawGrid();

    for (int i = 0; i < objects.size(); i++)
    {
        // cout << objects[i] << endl;
        objects[i]->draw();
    }

    for (int i = 0; i < point_lights.size(); i++)
    {
        point_lights[i]->draw();
    }

    for (int i = 0; i < spot_lights.size(); i++)
    {
        spot_lights[i]->draw();
    }

    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
}

void init()
{
    drawaxes = 0;
    drawgrid = 0;

    glClearColor(0, 0, 0, 0);

    pos = Vector3D(115, 115, 50);
    u = Vector3D(0.233214, -0.770819, 0.592832);
    r = Vector3D(0.930042, 0.354829, 0.0954915);
    l = Vector3D(-0.283961, 0.529089, 0.799645);

    loadData();

    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1, 1, 1000.0);
}

int main(int argc, char **argv)
{
    atexit(memoryCleanUp);

    glutInit(&argc, argv);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(750, 250);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("Ray Tracing");

    init();

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();

    // memoryCleanUp();

    return 0;
}