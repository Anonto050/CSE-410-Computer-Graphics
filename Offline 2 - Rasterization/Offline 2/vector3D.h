#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>

class Vector3D
{

private:

    double x;
    double y;
    double z;
    double w;

public:

    // Constructors and destructor
    Vector3D();
    Vector3D(double x, double y, double z);
    Vector3D(double x, double y, double z, double w);
    Vector3D(const Vector3D& vector);
    ~Vector3D();

    // Getters and setters
    double getX() const;
    double getY() const;
    double getZ() const;
    double getW() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setW(double w);

    //Magnitude
    double magnitude() const;

    // Normalize
    Vector3D normalize() const;

    //Scale - Divide by W
    Vector3D scale() const;

    // Dot and cross product
    double dotProduct(const Vector3D& vector) const;
    Vector3D crossProduct(const Vector3D& vector) const;

    // Operator overloading
    Vector3D operator+(const Vector3D& vector) const;
    Vector3D operator-(const Vector3D& vector) const;
    Vector3D operator*(double scalar) const;
    Vector3D operator/(double scalar) const;

    // Assignment operator
    Vector3D& operator=(const Vector3D& vector);

    // Stream insertion and extraction
    friend std::ostream& operator<<(std::ostream& stream, const Vector3D& vector);
    friend std::istream& operator>>(std::istream& stream, Vector3D& vector);

};

#endif // VECTOR3D_H