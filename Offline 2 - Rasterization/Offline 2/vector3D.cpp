#include "vector3D.h"
#include <iomanip>
#include <cmath>

// Constructors and destructor

Vector3D::Vector3D() : x(0.0), y(0.0), z(0.0), w(1.0) {}

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z), w(1.0) {}

Vector3D::Vector3D(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

Vector3D::Vector3D(const Vector3D &vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}

Vector3D::~Vector3D() {}

// Getters and setters
double Vector3D::getX() const { return x; }

double Vector3D::getY() const { return y; }

double Vector3D::getZ() const { return z; }

double Vector3D::getW() const { return w; }

void Vector3D::setX(double x) { this->x = x; }

void Vector3D::setY(double y) { this->y = y; }

void Vector3D::setZ(double z) { this->z = z; }

void Vector3D::setW(double w) { this->w = w; }

// Magnitude
double Vector3D::magnitude() const { return sqrt(x * x + y * y + z * z); }

// Normalize
Vector3D Vector3D::normalize() const
{
    double magnitude = this->magnitude();
    return Vector3D(x / magnitude, y / magnitude, z / magnitude);
}

// Scale - Divide by W
Vector3D Vector3D::scale() const
{
    return Vector3D(x / w, y / w, z / w);
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
    w = vector.w;
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
