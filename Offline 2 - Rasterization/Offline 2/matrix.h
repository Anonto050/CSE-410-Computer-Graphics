#ifndef MATRIX_H
#define MATRIX_H

#include "vector3D.h"

#define MATRIX_SIZE 4

class Matrix
{
private:
    double matrix[MATRIX_SIZE][MATRIX_SIZE];

public:
    // Constructors and destructor
    Matrix();
    ~Matrix();

    // Getters and setters
    double getMatrixValue(int row, int column) const;
    void setMatrixValue(int row, int column, double value);

    // Identity matrix
    Matrix createIdentityMatrix();

    // Determinant
    double determinant() const;

    // Matrix operations
    Matrix transpose() const;
    Matrix cofactor() const;
    Matrix adjugate() const;
    Matrix inverse() const;

    // Operator overloading
    Matrix operator+(const Matrix &matrix) const;
    Matrix operator-(const Matrix &matrix) const;
    Matrix operator*(const Matrix &matrix) const;
    Vector3D operator*(const Vector3D &vector) const;
    Matrix operator*(double scalar) const;
    Matrix operator/(double scalar) const;

    // Assignment operator
    Matrix &operator=(const Matrix &matrix);

    // Equality operator
    bool operator==(const Matrix &matrix) const;

    // Stream insertion and extraction
    friend std::ostream &operator<<(std::ostream &stream, const Matrix &matrix);
    friend std::istream &operator>>(std::istream &stream, Matrix &matrix);
};

#endif // MATRIX_H