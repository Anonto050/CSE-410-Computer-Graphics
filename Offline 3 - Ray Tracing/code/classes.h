#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <bits/stdc++.h>

#include "bitmap_image.hpp"

using namespace std;

#define PI acos(0.0) * 2
#define INV_SQRT_2 1 / sqrt(2)
#define AMBIENT 0
#define DIFFUSE 1
#define SPECULAR 2
#define REFLECTION 3
#define EPSILON 1e-8
#define INF 1e18


int recursion_level;

// Global functions

// Function to find determinant of a 3x3 matrix

double determinant(double a[3][3])
{
    return a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) - a[0][1] * (a[1][0] * a[2][2] - a[1][2] * a[2][0]) + a[0][2] * (a[1][0] * a[2][1] - a[1][1] * a[2][0]);
}

class Vector3D
{

private:
    double x;
    double y;
    double z;

public:
    // Constructors and destructor
    Vector3D();
    Vector3D(double x, double y, double z);
    Vector3D(const Vector3D &vector);
    ~Vector3D();

    // Getters and setters
    double getX() const;
    double getY() const;
    double getZ() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);

    // Magnitude
    double magnitude() const;

    // Square
    double square() const { return x * x + y * y + z * z; }

    // Normalize
    Vector3D normalize();

    // Dot and cross product
    double dotProduct(const Vector3D &vector) const;
    Vector3D crossProduct(const Vector3D &vector) const;

    // Operator overloading
    Vector3D operator+(const Vector3D &vector) const;
    Vector3D operator-(const Vector3D &vector) const;
    Vector3D operator-() const { return Vector3D(-x, -y, -z); }
    Vector3D operator*(double scalar) const;
    Vector3D operator*(const Vector3D &vector) const;
    Vector3D operator/(double scalar) const;

    // Assignment operator
    Vector3D &operator=(const Vector3D &vector);

    // Find distance between two points
    double distance(const Vector3D &vector) const { return (*this - vector).magnitude(); }

    // Stream insertion and extraction
    friend std::ostream &operator<<(std::ostream &stream, const Vector3D &vector);
    friend std::istream &operator>>(std::istream &stream, Vector3D &vector);
};

// Constructors and destructor

Vector3D::Vector3D() : x(0.0), y(0.0), z(0.0) {}

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

Vector3D::Vector3D(const Vector3D &vector) : x(vector.x), y(vector.y), z(vector.z) {}

Vector3D::~Vector3D() {}

// Getters and setters
double Vector3D::getX() const { return x; }

double Vector3D::getY() const { return y; }

double Vector3D::getZ() const { return z; }

void Vector3D::setX(double x) { this->x = x; }

void Vector3D::setY(double y) { this->y = y; }

void Vector3D::setZ(double z) { this->z = z; }

// Magnitude
double Vector3D::magnitude() const { return sqrt(square()); }

// Normalize
Vector3D Vector3D::normalize()
{
    double mag = magnitude();
    x /= mag;
    y /= mag;
    z /= mag;
    return *this;
}

// Dot and cross product
double Vector3D::dotProduct(const Vector3D &vector) const
{
    return x * vector.x + y * vector.y + z * vector.z;
}

Vector3D Vector3D::crossProduct(const Vector3D &vector) const
{
    return Vector3D(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
}

// Operator overloading
Vector3D Vector3D::operator+(const Vector3D &vector) const
{
    return Vector3D(x + vector.x, y + vector.y, z + vector.z);
}

Vector3D Vector3D::operator-(const Vector3D &vector) const
{
    return Vector3D(x - vector.x, y - vector.y, z - vector.z);
}

Vector3D Vector3D::operator*(const Vector3D &vector) const
{
    return Vector3D(x * vector.x, y * vector.y, z * vector.z);
}

Vector3D Vector3D::operator*(double scalar) const
{
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D Vector3D::operator/(double scalar) const
{
    return Vector3D(x / scalar, y / scalar, z / scalar);
}

// Assignment operator
Vector3D &Vector3D::operator=(const Vector3D &vector)
{
    x = vector.x;
    y = vector.y;
    z = vector.z;
    return *this;
}

// Stream insertion and extraction
std::ostream &operator<<(std::ostream &stream, const Vector3D &vector)
{
    // output doubles with 6 decimal places
    stream << std::fixed << std::setprecision(7) << vector.x << " " << vector.y << " " << vector.z;
    return stream;
}

std::istream &operator>>(std::istream &stream, Vector3D &vector)
{
    stream >> vector.x >> vector.y >> vector.z;
    return stream;
}

class Color
{
private:
    double r;
    double g;
    double b;

public:
    Color();
    Color(double r, double g, double b);
    Color(const Color &color);
    ~Color();

    double getR() const;
    double getG() const;
    double getB() const;

    void setR(double r);
    void setG(double g);
    void setB(double b);

    Color operator+(const Color &color) const;
    Color operator-(const Color &color) const;
    Color operator*(double scalar) const;
    Color operator*(const Color &color) const;
    Color operator/(double scalar) const;

    Color &operator=(const Color &color);

    void normalize();

    friend std::ostream &operator<<(std::ostream &stream, const Color &color);
    friend std::istream &operator>>(std::istream &stream, Color &color);
};

Color::Color() : r(0.0), g(0.0), b(0.0) {}

Color::Color(double r, double g, double b) : r(r), g(g), b(b) {}

Color::Color(const Color &color) : r(color.r), g(color.g), b(color.b) {}

Color::~Color() {}

// Getters and setters
double Color::getR() const { return r; }

double Color::getG() const { return g; }

double Color::getB() const { return b; }

void Color::setR(double r) { this->r = r; }

void Color::setG(double g) { this->g = g; }

void Color::setB(double b) { this->b = b; }

// Operator overloading
Color Color::operator+(const Color &color) const
{
    return Color(r + color.r, g + color.g, b + color.b);
}

Color Color::operator-(const Color &color) const
{
    return Color(r - color.r, g - color.g, b - color.b);
}

Color Color::operator*(double scalar) const
{
    return Color(r * scalar, g * scalar, b * scalar);
}

Color Color::operator*(const Color &color) const
{
    return Color(r * color.r, g * color.g, b * color.b);
}

Color Color::operator/(double scalar) const
{
    return Color(r / scalar, g / scalar, b / scalar);
}

// Assignment operator
Color &Color::operator=(const Color &color)
{
    r = color.r;
    g = color.g;
    b = color.b;
    return *this;
}

void Color::normalize()
{
    r = r > 1.0 ? 1.0 : r;
    r = r < 0.0 ? 0.0 : r;
    g = g > 1.0 ? 1.0 : g;
    g = g < 0.0 ? 0.0 : g;
    b = b > 1.0 ? 1.0 : b;
    b = b < 0.0 ? 0.0 : b;
}

// Stream insertion and extraction
std::ostream &operator<<(std::ostream &stream, const Color &color)
{
    // output doubles with 6 decimal places
    stream << std::fixed << std::setprecision(7) << color.r << " " << color.g << " " << color.b;
    return stream;
}

std::istream &operator>>(std::istream &stream, Color &color)
{
    stream >> color.r >> color.g >> color.b;
    return stream;
}

class Ray
{
private:
    Vector3D origin;
    Vector3D direction;

public:
    Ray();
    Ray(const Vector3D &origin, const Vector3D &direction);
    Ray(const Ray &ray);
    ~Ray();

    Vector3D getOrigin() const;
    Vector3D getDirection() const;

    void setOrigin(const Vector3D &origin);
    void setDirection(const Vector3D &direction);

    void normalize();

    Ray &operator=(const Ray &ray);

    friend std::ostream &operator<<(std::ostream &stream, const Ray &ray);
    friend std::istream &operator>>(std::istream &stream, Ray &ray);
};

Ray::Ray() : origin(Vector3D()), direction(Vector3D()) {}

Ray::Ray(const Vector3D &origin, const Vector3D &direction)
{
    this->origin = origin;
    this->direction = direction;
    this->normalize();
}

Ray::Ray(const Ray &ray) : origin(ray.origin), direction(ray.direction) {}

Ray::~Ray() {}

Vector3D Ray::getOrigin() const { return origin; }

Vector3D Ray::getDirection() const { return direction; }

void Ray::setOrigin(const Vector3D &origin) { this->origin = origin; }

void Ray::setDirection(const Vector3D &direction) { this->direction = direction; }

void Ray::normalize() { direction = direction.normalize(); }

Ray &Ray::operator=(const Ray &ray)
{
    origin = ray.origin;
    direction = ray.direction;
    return *this;
}

// Stream insertion and extraction
std::ostream &operator<<(std::ostream &stream, const Ray &ray)
{
    stream << ray.origin << " " << ray.direction;
    return stream;
}

std::istream &operator>>(std::istream &stream, Ray &ray)
{
    stream >> ray.origin >> ray.direction;
    return stream;
}

// Point light and Spot light classes

class PointLight
{
private:
    Vector3D position;
    Color color;

public:
    PointLight();
    PointLight(const Vector3D &position, const Color &color);
    PointLight(const Vector3D &position);
    PointLight(const PointLight &pointLight);
    ~PointLight();

    Vector3D getPosition() const;
    Color getColor() const;

    void setPosition(const Vector3D &position);
    void setColor(const Color &color);

    void draw() const;

    PointLight &operator=(const PointLight &pointLight);

    friend std::ostream &operator<<(std::ostream &stream, const PointLight &pointLight);
    friend std::istream &operator>>(std::istream &stream, PointLight &pointLight);
};

PointLight::PointLight() : position(Vector3D()), color(Color()) {}

PointLight::PointLight(const Vector3D &position, const Color &color) : position(position), color(color) {}

PointLight::PointLight(const Vector3D &position) : position(position), color(Color()) {}

PointLight::PointLight(const PointLight &pointLight) : position(pointLight.position), color(pointLight.color) {}

PointLight::~PointLight() {}

// Getters and setters
Vector3D PointLight::getPosition() const { return position; }

Color PointLight::getColor() const { return color; }

void PointLight::setPosition(const Vector3D &position) { this->position = position; }

void PointLight::setColor(const Color &color) { this->color = color; }

// Draw
void PointLight::draw() const
{
    glPushMatrix();
    glTranslatef(position.getX(), position.getY(), position.getZ());
    glColor3f(color.getR(), color.getG(), color.getB());
    glutSolidSphere(1, 50, 50);
    glPopMatrix();
}

PointLight &PointLight::operator=(const PointLight &pointLight)
{
    position = pointLight.position;
    color = pointLight.color;
    return *this;
}

// Stream insertion and extraction
std::ostream &operator<<(std::ostream &stream, const PointLight &pointLight)
{
    stream << pointLight.position << " " << pointLight.color;
    return stream;
}

std::istream &operator>>(std::istream &stream, PointLight &pointLight)
{
    stream >> pointLight.position >> pointLight.color;
    return stream;
}

class SpotLight
{
private:
    PointLight pointLight;
    Vector3D direction;
    double cutoff_angle;

public:
    SpotLight();
    SpotLight(const PointLight &pointLight, const Vector3D &direction, double cutoff_angle);
    SpotLight(const SpotLight &spotLight);
    ~SpotLight();

    PointLight getPointLight() const;
    Vector3D getDirection() const;
    double getCutoffAngle() const;

    void setPointLight(const PointLight &pointLight);
    void setDirection(const Vector3D &direction);
    void setCutoffAngle(double cutoff_angle);

    void draw() const;

    bool checkAngleWithRay(const Ray &ray) const;

    SpotLight &operator=(const SpotLight &spotLight);

    friend std::ostream &operator<<(std::ostream &stream, const SpotLight &spotLight);
    friend std::istream &operator>>(std::istream &stream, SpotLight &spotLight);
};

SpotLight::SpotLight() : pointLight(PointLight()), direction(Vector3D()), cutoff_angle(0.0) {}

SpotLight::SpotLight(const PointLight &pointLight, const Vector3D &direction, double cutoff_angle) : pointLight(pointLight), direction(direction), cutoff_angle(cutoff_angle) {}

SpotLight::SpotLight(const SpotLight &spotLight) : pointLight(spotLight.pointLight), direction(spotLight.direction), cutoff_angle(spotLight.cutoff_angle) {}

SpotLight::~SpotLight() {}

// Getters and setters
PointLight SpotLight::getPointLight() const { return pointLight; }

Vector3D SpotLight::getDirection() const { return direction; }

double SpotLight::getCutoffAngle() const { return cutoff_angle; }

void SpotLight::setPointLight(const PointLight &pointLight) { this->pointLight = pointLight; }

void SpotLight::setDirection(const Vector3D &direction) { this->direction = direction; }

void SpotLight::setCutoffAngle(double cutoff_angle) { this->cutoff_angle = cutoff_angle; }

// Draw
void SpotLight::draw() const
{
    pointLight.draw();
    
    glBegin(GL_LINES);
    {
        for(int i = -1; i <= 1; i += 1)
        {
            for(int j = -1; j <= 1; j += 1)
            {
                for(int k = -1; k <= 1; k += 1)
                {
                    glVertex3f(pointLight.getPosition().getX(), pointLight.getPosition().getY(), pointLight.getPosition().getZ());
                    glVertex3f(pointLight.getPosition().getX() + 2 * i, pointLight.getPosition().getY() + 2 * j, pointLight.getPosition().getZ() + 2 * k);
                }
            }
        }
    }
    glEnd();
}

bool SpotLight::checkAngleWithRay(const Ray &ray) const
{
    double dotProduct = direction.dotProduct(ray.getDirection());
    double angle = dotProduct / (direction.magnitude() * ray.getDirection().magnitude());
    angle = acos(abs(angle)) * 180 / PI;

    return angle <= cutoff_angle;
}

SpotLight &SpotLight::operator=(const SpotLight &spotLight)
{
    pointLight = spotLight.pointLight;
    direction = spotLight.direction;
    cutoff_angle = spotLight.cutoff_angle;
    return *this;
}

// Stream insertion and extraction
std::ostream &operator<<(std::ostream &stream, const SpotLight &spotLight)
{
    stream << spotLight.pointLight << " " << spotLight.direction << " " << spotLight.cutoff_angle;
    return stream;
}

std::istream &operator>>(std::istream &stream, SpotLight &spotLight)
{
    stream >> spotLight.pointLight >> spotLight.direction >> spotLight.cutoff_angle;
    return stream;
}

// Object class

class Object
{
private:
    Vector3D reference_point;
    Color color;
    double coEfficients[4];
    double height, width, length;
    int shine;

public:
    Object();
    Object(const Vector3D &reference_point);
    Object(const Vector3D &reference_point, double height, double width, double length);
    ~Object();

    Vector3D getReferencePoint() const;
    Color getColor() const;
    double *getCoEfficients();
    double getHeight() const;
    double getWidth() const;
    double getLength() const;
    int getShine() const;

    void setReferencePoint(const Vector3D &reference_point);
    void setColor(const Color &color);
    void setCoEfficients(double ambient, double diffuse, double specular, double reflection);
    void setHeight(double height);
    void setWidth(double width);
    void setLength(double length);
    void setShine(int shine);

    virtual void draw() const = 0;
    virtual Color getObjectColorAt(Vector3D &point);
    virtual Vector3D getNormalAt(Ray &ray, Vector3D &intersection_point);
    virtual double getTMin(Ray &ray);
    virtual double intersect(Ray &ray, Color &color, int level);
    void applyPhongIlluminationModel(Vector3D &intersection_point, Vector3D &normal, Ray &view, Color &final_color, Color &intersection_color, double t_present);

    Object &operator=(const Object &object);

    friend std::ostream &operator<<(std::ostream &stream, const Object &object);
    friend std::istream &operator>>(std::istream &stream, Object &object);
};


// List of objects and lights
vector<Object *> objects;
vector<PointLight *> point_lights;
vector<SpotLight *> spot_lights;

Object::Object() : reference_point(Vector3D()), color(Color()), height(0.0), width(0.0), length(0.0), shine(0)
{
    coEfficients[AMBIENT] = 0.0;
    coEfficients[DIFFUSE] = 0.0;
    coEfficients[SPECULAR] = 0.0;
    coEfficients[REFLECTION] = 0.0;
}

Object::Object(const Vector3D &reference_point) : reference_point(reference_point), color(Color()), height(0.0), width(0.0), length(0.0), shine(0)
{
    coEfficients[AMBIENT] = 0.0;
    coEfficients[DIFFUSE] = 0.0;
    coEfficients[SPECULAR] = 0.0;
    coEfficients[REFLECTION] = 0.0;
}

Object::Object(const Vector3D &reference_point, double height, double width, double length) : reference_point(reference_point), color(Color()), height(height), width(width), length(length), shine(0)
{
    coEfficients[AMBIENT] = 0.0;
    coEfficients[DIFFUSE] = 0.0;
    coEfficients[SPECULAR] = 0.0;
    coEfficients[REFLECTION] = 0.0;
}

Object::~Object() {}

// Getters and setters
Vector3D Object::getReferencePoint() const { return reference_point; }

Color Object::getColor() const { return color; }

double *Object::getCoEfficients() { return coEfficients; }

double Object::getHeight() const { return height; }

double Object::getWidth() const { return width; }

double Object::getLength() const { return length; }

int Object::getShine() const { return shine; }

void Object::setReferencePoint(const Vector3D &reference_point) { this->reference_point = reference_point; }

void Object::setColor(const Color &color) { this->color = color; }

void Object::setCoEfficients(double ambient, double diffuse, double specular, double reflection)
{
    coEfficients[AMBIENT] = ambient;
    coEfficients[DIFFUSE] = diffuse;
    coEfficients[SPECULAR] = specular;
    coEfficients[REFLECTION] = reflection;
}

void Object::setHeight(double height) { this->height = height; }

void Object::setWidth(double width) { this->width = width; }

void Object::setLength(double length) { this->length = length; }

void Object::setShine(int shine) { this->shine = shine; }

Object &Object::operator=(const Object &object)
{
    reference_point = object.reference_point;
    color = object.color;
    coEfficients[AMBIENT] = object.coEfficients[AMBIENT];
    coEfficients[DIFFUSE] = object.coEfficients[DIFFUSE];
    coEfficients[SPECULAR] = object.coEfficients[SPECULAR];
    coEfficients[REFLECTION] = object.coEfficients[REFLECTION];
    height = object.height;
    width = object.width;
    length = object.length;
    shine = object.shine;
    return *this;
}

// virtual functions

void Object::draw() const {}

Color Object::getObjectColorAt(Vector3D &point) { return color; }

Vector3D Object::getNormalAt(Ray &ray, Vector3D &intersection_point) { return Vector3D(); }

double Object::getTMin(Ray &ray) { return -1.0; }

void Object::applyPhongIlluminationModel(Vector3D &intersection_point, Vector3D &normal, Ray &view, Color &final_color, Color &intersection_color, double t_present)
{

    final_color = intersection_color * coEfficients[AMBIENT];
    final_color.normalize();

    for (int i = 0; i < point_lights.size(); i++)
    {

        Ray light_ray = Ray(point_lights[i]->getPosition(), intersection_point - point_lights[i]->getPosition());
        light_ray.normalize();

        double t_min = INF;

        for (int j = 0; j < objects.size(); j++)
        {
            double t_current = objects[j]->intersect(light_ray, color, 0);

            if (t_current > 0 && (t_min - t_current) > EPSILON)
            {
                t_min = t_current;
            }
        }

        if ((t_present - t_min) < EPSILON)
        {
            double lambert = normal.dotProduct(-light_ray.getDirection());


            Ray reflected_ray = Ray(intersection_point, (normal * (2 * lambert)) + light_ray.getDirection());
            reflected_ray.normalize();

            double phong = reflected_ray.getDirection().dotProduct(-view.getDirection());
            phong = pow(phong, shine);

            lambert = lambert < 0 ? 0 : lambert;
            phong = phong < 0 ? 0 : phong;

            final_color = final_color + ((point_lights[i]->getColor() * intersection_color) * (coEfficients[DIFFUSE] * lambert));
            final_color.normalize();
            final_color = final_color +  (point_lights[i]->getColor() * (coEfficients[SPECULAR] * phong));
            final_color.normalize();
        }
    }

    for (int i = 0; i < spot_lights.size(); i++)
    {
        if (!spot_lights[i]->checkAngleWithRay(Ray(spot_lights[i]->getPointLight().getPosition(), intersection_point - spot_lights[i]->getPointLight().getPosition())))
        {
            // light_ray.setOrigin(spot_lights[i]->getPointLight().getPosition());
            // light_ray.setDirection(intersection_point - spot_lights[i]->getPointLight().getPosition());
            // light_ray.normalize();
            Ray light_ray = Ray(spot_lights[i]->getPointLight().getPosition(), intersection_point - spot_lights[i]->getPointLight().getPosition());
            light_ray.normalize();

            double t_min = INF;

            for (int j = 0; j < objects.size(); j++)
            {
                double t_current = objects[j]->intersect(light_ray, color, 0);

                if (t_current > 0 && (t_min - t_current) > EPSILON)
                {
                    t_min = t_current;
                }
            }

            if ((t_present - t_min) < EPSILON)
            {
                double lambert = normal.dotProduct(-light_ray.getDirection());

                // reflected_ray.setOrigin(intersection_point);
                // reflected_ray.setDirection((normal * (2 * lambert)) + light_ray.getDirection());
                // reflected_ray.normalize();
                Ray reflected_ray = Ray(intersection_point, (normal * (2 * lambert)) + light_ray.getDirection());
                reflected_ray.normalize();

                double phong = reflected_ray.getDirection().dotProduct(-view.getDirection());
                phong = pow(phong, shine);

                lambert = lambert < 0 ? 0 : lambert;
                phong = phong < 0 ? 0 : phong;

                final_color = final_color + ((spot_lights[i]->getPointLight().getColor() * intersection_color) * (coEfficients[DIFFUSE] * lambert));
                final_color.normalize();
                final_color = final_color + (spot_lights[i]->getPointLight().getColor() * (coEfficients[SPECULAR] * phong));
                final_color.normalize();
            }
        }
    }

    // if(coEfficients[REFLECTION] > 0 && t_present > EPSILON)
    // {
    //     reflected_ray.setOrigin(intersection_point);
    //     reflected_ray.setDirection((normal * 2 * normal.dotProduct(-view.getDirection())) + view.getDirection());
    //     reflected_ray.normalize();

    //     Color reflected_color;
    //     double t_min = INF;
    //     int index = -1;

    //     for (int i = 0; i < objects.size(); i++)
    //     {
    //         double t_current = objects[i]->intersect(reflected_ray, reflected_color, 0);

    //         if(t_current > 0 && (t_min - t_current) > EPSILON)
    //         {
    //             t_min = t_current;
    //             index = i;
    //         }
    //     }

    //     if(index != -1)
    //     {
    //         Vector3D intersection_point_reflected = reflected_ray.getOrigin() + (reflected_ray.getDirection() * t_min);
    //         reflected_color = reflected_color * coEfficients[REFLECTION];
    //         reflected_color.normalize();
    //         final_color = final_color + (reflected_color * objects[index]->getObjectColorAt(intersection_point_reflected));
    //         final_color.normalize();
    //     }
    // }

    return;
}

double Object::intersect(Ray &ray, Color &color, int level = 0)
{

    ray.normalize();

    double t = getTMin(ray);

    if (t < 0)
    {
        return t;
    }

    if (level == 0)
    {
        return t;
    }

    Vector3D intersection_point = ray.getOrigin() + (ray.getDirection() * t);
    Vector3D normal = getNormalAt(ray, intersection_point);

    Color intersection_color = getObjectColorAt(intersection_point);

    applyPhongIlluminationModel(intersection_point, normal, ray, color, intersection_color, t);

    if(level >= recursion_level)
    {
        return t;
    }

    Ray reflected_ray;
    reflected_ray.setOrigin(intersection_point);
    reflected_ray.setDirection((normal * (2 * normal.dotProduct(-ray.getDirection()))) + ray.getDirection());

    // slightly move the origin of the reflected ray to avoid self-intersection
    reflected_ray.setOrigin(reflected_ray.getOrigin() + (reflected_ray.getDirection() * 0.00001));  
    reflected_ray.normalize();

    Color reflected_color;
    double t_min = INF;
    int index = -1;

    for (int i = 0; i < objects.size(); i++)
    {
        double t_current = objects[i]->intersect(reflected_ray, reflected_color, 0);

        if (t_current >= 0 && (t_min - t_current) > EPSILON)
        {
            t_min = t_current;
            index = i;
        }
    }

    if (abs(t_min - INF) > EPSILON)
    {
        objects[index]->intersect(reflected_ray, reflected_color, level + 1);
        color = color + (reflected_color * coEfficients[REFLECTION]);
        color.normalize();
    }

    return t;
}

// Stream insertion and extraction
std::ostream &operator<<(std::ostream &stream, const Object &object)
{
    stream << object.reference_point << " " << object.color << " " << object.coEfficients[AMBIENT] << " " << object.coEfficients[DIFFUSE] << " " << object.coEfficients[SPECULAR] << " " << object.coEfficients[REFLECTION] << " " << object.height << " " << object.width << " " << object.length << " " << object.shine;
    return stream;
}

std::istream &operator>>(std::istream &stream, Object &object)
{
    stream >> object.reference_point >> object.color >> object.coEfficients[AMBIENT] >> object.coEfficients[DIFFUSE] >> object.coEfficients[SPECULAR] >> object.coEfficients[REFLECTION] >> object.height >> object.width >> object.length >> object.shine;
    return stream;
}


// Triangle class

class Triangle : public Object
{
private:
    Vector3D a;
    Vector3D b;
    Vector3D c;

public:
    Triangle();
    Triangle(const Vector3D &a, const Vector3D &b, const Vector3D &c);
    ~Triangle();

    Vector3D getA() const;
    Vector3D getB() const;
    Vector3D getC() const;

    void setA(const Vector3D &a);
    void setB(const Vector3D &b);
    void setC(const Vector3D &c);

    void draw() const;
    Color getObjectColorAt(Vector3D &point) ;
    Vector3D getNormalAt(Ray &ray, Vector3D &intersection_point) ;
    double getTMin(Ray &ray) ;
    // double intersect(Ray& ray, Color& color, int level);

    Triangle &operator=(const Triangle &triangle);

    friend std::ostream &operator<<(std::ostream &stream, const Triangle &triangle);
    friend std::istream &operator>>(std::istream &stream, Triangle &triangle);
};

Triangle::Triangle() : a(Vector3D()), b(Vector3D()), c(Vector3D()) {}

Triangle::Triangle(const Vector3D &a, const Vector3D &b, const Vector3D &c) : a(a), b(b), c(c) {}

Triangle::~Triangle() {}

// Getters and setters
Vector3D Triangle::getA() const { return a; }

Vector3D Triangle::getB() const { return b; }

Vector3D Triangle::getC() const { return c; }

void Triangle::setA(const Vector3D &a) { this->a = a; }

void Triangle::setB(const Vector3D &b) { this->b = b; }

void Triangle::setC(const Vector3D &c) { this->c = c; }

// Draw
void Triangle::draw() const
{
    glPushMatrix();
    {
        glColor3f(getColor().getR(), getColor().getG(), getColor().getB());
        glBegin(GL_TRIANGLES);
        {
            // cout << getColor().getR() << " " << getColor().getG() << " " << getColor().getB() << endl;
            glVertex3f(a.getX(), a.getY(), a.getZ());
            glVertex3f(b.getX(), b.getY(), b.getZ());
            glVertex3f(c.getX(), c.getY(), c.getZ());
        }
        glEnd();
    }
    glPopMatrix();
}

Color Triangle::getObjectColorAt(Vector3D &point) 
{
    return getColor();
}

Vector3D Triangle::getNormalAt(Ray &ray, Vector3D &intersection_point) 
{
    Vector3D normal = (a - b).crossProduct(c - b);

    normal = normal.normalize();

    if (normal.dotProduct(ray.getDirection()) > 0)
    {
        normal = normal * -1;
    }

    return normal;
}

double Triangle::getTMin(Ray &ray) 
{
    // Intersection with Barycentric coordinates using Cramer's rule
    Vector3D c1 = a - b;
    Vector3D c2 = a - c;
    Vector3D c3 = ray.getDirection();

    Vector3D c4 = a - ray.getOrigin();

    // Form the A matrix
    double A[3][3] = {{c1.getX(), c2.getX(), c3.getX()},
                      {c1.getY(), c2.getY(), c3.getY()},
                      {c1.getZ(), c2.getZ(), c3.getZ()}};

    // Determinant of A
    double detA = determinant(A);

    if (abs(detA) < EPSILON)
    {
        return -1;
    }

    // Form the beta matrix
    double beta[3][3] = {{c4.getX(), c2.getX(), c3.getX()},
                         {c4.getY(), c2.getY(), c3.getY()},
                         {c4.getZ(), c2.getZ(), c3.getZ()}};

    // Determinant of beta
    double detBeta = determinant(beta);

    // Form the gamma matrix
    double gamma[3][3] = {{c1.getX(), c4.getX(), c3.getX()},
                          {c1.getY(), c4.getY(), c3.getY()},
                          {c1.getZ(), c4.getZ(), c3.getZ()}};

    // Determinant of gamma
    double detGamma = determinant(gamma);

    // Form the t matrix
    double t[3][3] = {{c1.getX(), c2.getX(), c4.getX()},
                      {c1.getY(), c2.getY(), c4.getY()},
                      {c1.getZ(), c2.getZ(), c4.getZ()}};

    // Determinant of t
    double detT = determinant(t);

    // cout << detA << " " << detBeta << " " << detGamma << " " << detT << endl;

    double betaValue = detBeta / detA;

    double gammaValue = detGamma / detA;

    double tValue = detT / detA;

    if (betaValue < 0 || (betaValue - 1) > EPSILON || gammaValue < 0 || (gammaValue - 1) > EPSILON || (betaValue + gammaValue - 1) > EPSILON || tValue < 0)
    {
        return -1;
    }

    return tValue;

    // double detA = (a.getX() - b.getX()) * ((a.getY() - c.getY()) * ray.getDirection().getZ() - (a.getZ() - c.getZ()) * ray.getDirection().getY()) - (a.getY() - b.getY()) * ((a.getX() - c.getX()) * ray.getDirection().getZ() - (a.getZ() - c.getZ()) * ray.getDirection().getX()) + (a.getZ() - b.getZ()) * ((a.getX() - c.getX()) * ray.getDirection().getY() - (a.getY() - c.getY()) * ray.getDirection().getX());

    // if(abs(detA) < EPSILON)
    // {
    //     return -1;
    // }

    // double detBeta = (a.getX() - ray.getOrigin().getX()) * ((a.getY() - c.getY()) * ray.getDirection().getZ() - (a.getZ() - c.getZ()) * ray.getDirection().getY()) - (a.getY() - ray.getOrigin().getY()) * ((a.getX() - c.getX()) * ray.getDirection().getZ() - (a.getZ() - c.getZ()) * ray.getDirection().getX()) + (a.getZ() - ray.getOrigin().getZ()) * ((a.getX() - c.getX()) * ray.getDirection().getY() - (a.getY() - c.getY()) * ray.getDirection().getX());

    // double detGamma = (a.getX() - b.getX()) * ((a.getY() - ray.getOrigin().getY()) * (a.getZ() - c.getZ()) - (a.getY() - c.getY()) * (a.getZ() - ray.getOrigin().getZ())) - (a.getY() - b.getY()) * ((a.getX() - ray.getOrigin().getX()) * (a.getZ() - c.getZ()) - (a.getX() - c.getX()) * (a.getZ() - ray.getOrigin().getZ())) + (a.getZ() - b.getZ()) * ((a.getX() - ray.getOrigin().getX()) * (a.getY() - c.getY()) - (a.getX() - c.getX()) * (a.getY() - ray.getOrigin().getY()));

    // double detT = (a.getX() - b.getX()) * ((a.getY() - c.getY()) * (a.getZ() - ray.getOrigin().getZ()) - (a.getY() - ray.getOrigin().getY()) * (a.getZ() - c.getZ())) - (a.getY() - b.getY()) * ((a.getX() - c.getX()) * (a.getZ() - ray.getOrigin().getZ()) - (a.getX() - ray.getOrigin().getX()) * (a.getZ() - c.getZ())) + (a.getZ() - b.getZ()) * ((a.getX() - c.getX()) * (a.getY() - ray.getOrigin().getY()) - (a.getX() - ray.getOrigin().getX()) * (a.getY() - c.getY()));

    // double beta = detBeta / detA;

    // double gamma = detGamma / detA;

    // double tValue = detT / detA;

    // if (beta < 0 || (beta - 1) > EPSILON || gamma < 0 || (gamma - 1) > EPSILON || (beta + gamma - 1) > EPSILON || tValue < 0)
    // {
    //     return -1;
    // }

    // return tValue;
}

Triangle &Triangle::operator=(const Triangle &triangle)
{
    a = triangle.a;
    b = triangle.b;
    c = triangle.c;
    return *this;
}

// Stream insertion and extraction
std::ostream &operator<<(std::ostream &stream, const Triangle &triangle)
{
    stream << triangle.a << " " << triangle.b << " " << triangle.c;
    return stream;
}

std::istream &operator>>(std::istream &stream, Triangle &triangle)
{
    stream >> triangle.a >> triangle.b >> triangle.c;
    return stream;
}

// Sphere class

class Sphere : public Object
{
private:
    double radius;
    Vector3D center;

public:
    Sphere();
    Sphere(const Vector3D &center, double radius);
    ~Sphere();

    double getRadius() const;
    Vector3D getCenter() const;

    void setRadius(double radius);
    void setCenter(const Vector3D &center);

    void draw() const;
    Color getObjectColorAt(Vector3D &point);
    Vector3D getNormalAt(Ray &ray, Vector3D &intersection_point);
    double getTMin(Ray &ray) ;
    // double intersect(Ray &ray, Color &color, int level);

    Sphere &operator=(const Sphere &sphere);

    friend std::ostream &operator<<(std::ostream &stream, const Sphere &sphere);
    friend std::istream &operator>>(std::istream &stream, Sphere &sphere);
};

Sphere::Sphere() : radius(0.0), center(Vector3D()) {}

Sphere::Sphere(const Vector3D &center, double radius)
{
    this->center = center;
    this->radius = radius;

    setReferencePoint(center);
    setHeight(radius);
    setWidth(radius);
    setLength(radius);
}

Sphere::~Sphere() {}

// Getters and setters
double Sphere::getRadius() const { return radius; }

Vector3D Sphere::getCenter() const { return center; }

void Sphere::setRadius(double radius) { this->radius = radius; }

void Sphere::setCenter(const Vector3D &center) { this->center = center; }

// Draw

void drawSphere(double radius, int slices, int stacks)
{
    Vector3D points[100][100];
    int i, j;
    double h, r;
    // generate points
    for (i = 0; i <= stacks; i++)
    {
        h = radius * sin(((double)i / (double)stacks) * (PI / 2));
        r = radius * cos(((double)i / (double)stacks) * (PI / 2));
        for (j = 0; j <= slices; j++)
        {
            points[i][j].setX(r * cos(((double)j / (double)slices) * 2 * PI));
            points[i][j].setY(r * sin(((double)j / (double)slices) * 2 * PI));
            points[i][j].setZ(h);
        }
    }
    // draw quads using generated points
    for (i = 0; i < stacks; i++)
    {
        for (j = 0; j < slices; j++)
        {
            glBegin(GL_QUADS);
            {
                // upper hemisphere
                glVertex3f(points[i][j].getX(), points[i][j].getY(), points[i][j].getZ());
                glVertex3f(points[i][j + 1].getX(), points[i][j + 1].getY(), points[i][j + 1].getZ());
                glVertex3f(points[i + 1][j + 1].getX(), points[i + 1][j + 1].getY(), points[i + 1][j + 1].getZ());
                glVertex3f(points[i + 1][j].getX(), points[i + 1][j].getY(), points[i + 1][j].getZ());

                // lower hemisphere
                glVertex3f(points[i][j].getX(), points[i][j].getY(), -points[i][j].getZ());
                glVertex3f(points[i][j + 1].getX(), points[i][j + 1].getY(), -points[i][j + 1].getZ());
                glVertex3f(points[i + 1][j + 1].getX(), points[i + 1][j + 1].getY(), -points[i + 1][j + 1].getZ());
                glVertex3f(points[i + 1][j].getX(), points[i + 1][j].getY(), -points[i + 1][j].getZ());
            }
            glEnd();
        }
    }
}

void Sphere::draw() const
{
    glPushMatrix();
    {
        glTranslatef(center.getX(), center.getY(), center.getZ());
        glColor3f(getColor().getR(), getColor().getG(), getColor().getB());
        drawSphere(radius, 20, 24);
    }
    glPopMatrix();
}



Color Sphere::getObjectColorAt(Vector3D &point) 
{
    return getColor();
}

Vector3D Sphere::getNormalAt(Ray &ray, Vector3D &intersection_point)
{
    Vector3D normal = intersection_point - center;
    normal = normal.normalize();

    // If the eye is outside the sphere, the normal is in the opposite direction
    if ((ray.getOrigin() - center).square() - radius * radius > EPSILON)
    {
        normal = normal * -1;
    }

    return normal;
}

double Sphere::getTMin(Ray &ray) 
{
    Vector3D temp = ray.getOrigin() - center;

    double a = 1;
    double b = 2 * (ray.getDirection().dotProduct(temp));
    double c = temp.dotProduct(temp) - radius * radius;

    double d = b * b - 4 * a * c;

    if (d < 0)
    {
        return -1;
    }

    double t1 = (-b + sqrt(d)) / (2 * a);
    double t2 = (-b - sqrt(d)) / (2 * a);

    if (t1 < 0 && t2 < 0)
    {
        return -1;
    }

    if (t1 > 0 && t2 > 0)
    {
        return t2;
    }

    if (t1 > 0)
    {
        return t1;
    }

    return t2;

    // Vector3D R_o = ray.getOrigin() - center;
    // ray.normalize();

    // double tp = -R_o.dotProduct(ray.getDirection());

    // if(tp < 0)
    // {
    //     return -1;
    // }

    // double d2 = R_o.dotProduct(R_o) - tp * tp;

    // if(d2 - radius * radius > EPSILON)
    // {
    //     return -1;
    // }
    
    // double t_prime = sqrt(radius * radius - d2);

    // double t1 = tp + t_prime;
    // double t2 = tp - t_prime;

    // double t = -1;

    // if((R_o.square() - radius * radius) > EPSILON)
    // {
    //     if(t2 >= 0)
    //     {
    //         t = t2;
    //     }
    // }
    // else
    // {
    //     if(t1 >= 0)
    //     {
    //         t = t1;
    //     }
    // }

    // return t;
}

Sphere &Sphere::operator=(const Sphere &sphere)
{
    radius = sphere.radius;
    center = sphere.center;
    return *this;
}

// Stream insertion and extraction
std::ostream &operator<<(std::ostream &stream, const Sphere &sphere)
{
    stream << sphere.center << " " << sphere.radius;
    return stream;
}

std::istream &operator>>(std::istream &stream, Sphere &sphere)
{
    stream >> sphere.center >> sphere.radius;
    return stream;
}

// General class

class General : public Object
{

public:
    double A, B, C, D, E, F, G, H, I, J;
    General();
    General(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J, const Vector3D &reference_point, double height, double width, double length);
    ~General();

    bool isInside(Vector3D &point) const;

    void draw() const;
    Color getObjectColorAt(Vector3D &point) ;
    Vector3D getNormalAt(Ray &ray, Vector3D &intersection_point) ;
    double getTMin(Ray &ray) ;
    // double intersect(Ray &ray, Color &color, int level);

    General &operator=(const General &general);

    friend std::ostream &operator<<(std::ostream &stream, const General &general);
    friend std::istream &operator>>(std::istream &stream, General &general);
};

General::General() : A(0.0), B(0.0), C(0.0), D(0.0), E(0.0), F(0.0), G(0.0), H(0.0), I(0.0), J(0.0) {}

General::General(double A, double B, double C, double D, double E, double F, double G, double H, double I, double J, const Vector3D &reference_point, double height, double width, double length) : A(A), B(B), C(C), D(D), E(E), F(F), G(G), H(H), I(I), J(J)
{
    setReferencePoint(reference_point);
    setHeight(height);
    setWidth(width);
    setLength(length);
}

General::~General() {}

bool General::isInside(Vector3D &point) const
{
    if (this->getLength() > 0)
    {
        if (point.getX() < getReferencePoint().getX() || point.getX() > getReferencePoint().getX() + getLength())
        {
            return false;
        }
    }

    if (this->getWidth() > 0)
    {
        if (point.getY() < getReferencePoint().getY() || point.getY() > getReferencePoint().getY() + getWidth())
        {
            return false;
        }
    }

    if (this->getHeight() > 0)
    {
        if (point.getZ() < getReferencePoint().getZ() || point.getZ() > getReferencePoint().getZ() + getHeight())
        {
            return false;
        }
    }

    return true;
}

void General::draw() const
{
}

Color General::getObjectColorAt(Vector3D &point) 
{
    return getColor();
}

Vector3D General::getNormalAt(Ray &ray, Vector3D &intersection_point) 
{
    Vector3D normal;
    normal.setX(2 * A * intersection_point.getX() + D * intersection_point.getY() + E * intersection_point.getZ() + G);
    normal.setY(2 * B * intersection_point.getY() + D * intersection_point.getX() + F * intersection_point.getZ() + H);
    normal.setZ(2 * C * intersection_point.getZ() + E * intersection_point.getX() + F * intersection_point.getY() + I);
    normal = normal.normalize();

    if (normal.dotProduct(ray.getOrigin() - this->getReferencePoint()) > 0)
    {
        normal = normal * -1;
    }

    return normal;
}

double General::getTMin(Ray &ray) 
{
    double R_ox = ray.getOrigin().getX();
    double R_oy = ray.getOrigin().getY();
    double R_oz = ray.getOrigin().getZ();

    double R_dx = ray.getDirection().getX();
    double R_dy = ray.getDirection().getY();
    double R_dz = ray.getDirection().getZ();

    double a = A * R_dx * R_dx + B * R_dy * R_dy + C * R_dz * R_dz + D * R_dx * R_dy + E * R_dx * R_dz + F * R_dy * R_dz;
    double b = 2 * (A * R_ox * R_dx + B * R_oy * R_dy + C * R_oz * R_dz) + D * (R_ox * R_dy + R_oy * R_dx) + E * (R_ox * R_dz + R_oz * R_dx) + F * (R_oy * R_dz + R_oz * R_dy) + G * R_dx + H * R_dy + I * R_dz;
    double c = A * R_ox * R_ox + B * R_oy * R_oy + C * R_oz * R_oz + D * R_ox * R_oy + E * R_ox * R_oz + F * R_oy * R_oz + G * R_ox + H * R_oy + I * R_oz + J;

    double d = b * b - 4 * a * c;

    if (d < 0)
    {
        return -1;
    }

    double t1 = (-b - sqrt(d)) / (2 * a);
    double t2 = (-b + sqrt(d)) / (2 * a);

    Vector3D intersection_point1 = ray.getOrigin() + (ray.getDirection() * t1);
    Vector3D intersection_point2 = ray.getOrigin() + (ray.getDirection() * t2);

    if (t1 < 0 && t2 < 0)
    {
        return -1;
    }

    if (t1 > 0)
    {
        if (isInside(intersection_point1))
        {
            return t1;
        }
    }

    if (t2 > 0)
    {
        if (isInside(intersection_point2))
        {
            return t2;
        }
    }

    return -1;
}

General &General::operator=(const General &general)
{
    A = general.A;
    B = general.B;
    C = general.C;
    D = general.D;
    E = general.E;
    F = general.F;
    G = general.G;
    H = general.H;
    I = general.I;
    J = general.J;
    return *this;
}

// Stream insertion and extraction
std::ostream &operator<<(std::ostream &stream, const General &general)
{
    stream << general.A << " " << general.B << " " << general.C << " " << general.D << " " << general.E << " " << general.F << " " << general.G << " " << general.H << " " << general.I << " " << general.J;
    return stream;
}

std::istream &operator>>(std::istream &stream, General &general)
{
    stream >> general.A >> general.B >> general.C >> general.D >> general.E >> general.F >> general.G >> general.H >> general.I >> general.J ;
    return stream;
}

// Floor class

class Floor : public Object
{
private:
    double tile_width;
    double floor_width;
    Color secondColor; // for checkerboard pattern

public:
    Floor();
    Floor(double tile_width, double floor_width);
    ~Floor();

    double getTileWidth() const;
    double getFloorWidth() const;
    Color getSecondColor() const;

    void setTileWidth(double tile_width);
    void setFloorWidth(double floor_width);
    void setSecondColor(const Color &secondColor);

    void draw() const;
    Color getObjectColorAt(Vector3D &point) ;
    Vector3D getNormalAt(Ray &ray, Vector3D &intersection_point) ;
    double getTMin(Ray &ray) ;
    // double intersect(Ray &ray, Color &color, int level);

    Floor &operator=(const Floor &floor);

    friend std::ostream &operator<<(std::ostream &stream, const Floor &floor);
    friend std::istream &operator>>(std::istream &stream, Floor &floor);
};

Floor::Floor() : tile_width(0.0), floor_width(0.0), secondColor(Color()) {}

Floor::Floor(double tile_width, double floor_width) : tile_width(tile_width), floor_width(floor_width) {
    setReferencePoint(Vector3D(-floor_width / 2, -floor_width / 2, 0));
    setLength(floor_width);

    this->setColor(Color(1.0, 1.0, 1.0));

    secondColor = Color(0.0, 0.0, 0.0);
}

Floor::~Floor() {}

// Getters and setters
double Floor::getTileWidth() const { return tile_width; }

double Floor::getFloorWidth() const { return floor_width; }

Color Floor::getSecondColor() const { return secondColor; }

void Floor::setTileWidth(double tile_width) { this->tile_width = tile_width; }

void Floor::setFloorWidth(double floor_width) { this->floor_width = floor_width; }

void Floor::setSecondColor(const Color &secondColor) { this->secondColor = secondColor; }

// Draw
void Floor::draw() const
{
    int tile_count = floor_width / tile_width;

    glPushMatrix();
    {
        glBegin(GL_QUADS);
        {
            for (int i = 0; i < tile_count; i++)
            {
                for (int j = 0; j < tile_count; j++)
                {
                    if ((i + j) % 2 == 0)
                    {
                        glColor3f(getColor().getR(), getColor().getG(), getColor().getB());
                    }
                    else
                    {
                        glColor3f(secondColor.getR(), secondColor.getG(), secondColor.getB());
                    }

                    glVertex3f(getReferencePoint().getX() + i * tile_width, getReferencePoint().getY() + j * tile_width, getReferencePoint().getZ());
                    glVertex3f(getReferencePoint().getX() + (i + 1) * tile_width, getReferencePoint().getY() + j * tile_width, getReferencePoint().getZ());
                    glVertex3f(getReferencePoint().getX() + (i + 1) * tile_width, getReferencePoint().getY() + (j + 1) * tile_width, getReferencePoint().getZ());
                    glVertex3f(getReferencePoint().getX() + i * tile_width, getReferencePoint().getY() + (j + 1) * tile_width, getReferencePoint().getZ());
                }
            }
        }
        glEnd();
    }
    glPopMatrix();

}

Color Floor::getObjectColorAt(Vector3D &point)
{
    int i = (point.getX() - getReferencePoint().getX()) / tile_width;
    int j = (point.getY() - getReferencePoint().getY()) / tile_width;

    if ((i + j) % 2 == 0)
    {
        return getColor();
    }
    else
    {
        return secondColor;
    }
}

Vector3D Floor::getNormalAt(Ray &ray, Vector3D &intersection_point) 
{
    Vector3D normal = Vector3D(0, 0, 1);

    if(ray.getOrigin().getZ() < 0)
    {
        normal = normal * -1;
    }

    return normal;
}

double Floor::getTMin(Ray &ray) 
{
    Vector3D normal = Vector3D(0, 0, 1);

    if(ray.getDirection().dotProduct(normal) == 0)
    {
        return -1;
    }

    double t = (-ray.getOrigin().dotProduct(normal)) / (ray.getDirection().dotProduct(normal));

    Vector3D intersection_point = ray.getOrigin() + (ray.getDirection() * t);

    if(intersection_point.getX() < getReferencePoint().getX() || intersection_point.getX() > getReferencePoint().getX() + getLength() || intersection_point.getY() < getReferencePoint().getY() || intersection_point.getY() > getReferencePoint().getY() + getLength())
    {
        return -1;
    }

    return t;
}

Floor &Floor::operator=(const Floor &floor)
{
    tile_width = floor.tile_width;
    floor_width = floor.floor_width;
    secondColor = floor.secondColor;
    return *this;
}

// Stream insertion and extraction
std::ostream &operator<<(std::ostream &stream, const Floor &floor)
{
    stream << floor.tile_width << " " << floor.floor_width << " " << floor.secondColor;
    return stream;
}

std::istream &operator>>(std::istream &stream, Floor &floor)
{
    stream >> floor.tile_width >> floor.floor_width >> floor.secondColor;
    return stream;
}