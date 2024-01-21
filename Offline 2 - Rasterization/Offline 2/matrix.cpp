#include "matrix.h"

// Constructors and destructor

Matrix::Matrix()
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            matrix[i][j] = 0.0;
        }
    }
}

Matrix::~Matrix() {}

// Getters and setters

double Matrix::getMatrixValue(int row, int column) const
{
    return matrix[row][column];
}

void Matrix::setMatrixValue(int row, int column, double value)
{
    matrix[row][column] = value;
}

//Identity matrix
Matrix Matrix::createIdentityMatrix()
{
    Matrix identityMatrix;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        identityMatrix.matrix[i][i] = 1.0;
    }

    return identityMatrix;
}

//Determinant
double Matrix::determinant() const
{
    double determinant = 0.0;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        determinant += matrix[0][i] * (matrix[1][(i + 1) % 4] * (matrix[2][(i + 2) % 4] * matrix[3][(i + 3) % 4] - matrix[2][(i + 3) % 4] * matrix[3][(i + 2) % 4]) -
                                       matrix[1][(i + 2) % 4] * (matrix[2][(i + 1) % 4] * matrix[3][(i + 3) % 4] - matrix[2][(i + 3) % 4] * matrix[3][(i + 1) % 4]) +
                                       matrix[1][(i + 3) % 4] * (matrix[2][(i + 1) % 4] * matrix[3][(i + 2) % 4] - matrix[2][(i + 2) % 4] * matrix[3][(i + 1) % 4]));
    }

    return determinant;
}

// Matrix operations
Matrix Matrix::transpose() const
{
    Matrix transposeMatrix;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            transposeMatrix.matrix[i][j] = matrix[j][i];
        }
    }

    return transposeMatrix;
}

Matrix Matrix::cofactor() const
{
    Matrix cofactorMatrix;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            cofactorMatrix.matrix[i][j] = pow(-1.0, i + j) * (matrix[(i + 1) % 4][(j + 1) % 4] * matrix[(i + 2) % 4][(j + 2) % 4] * matrix[(i + 3) % 4][(j + 3) % 4] +
                                                              matrix[(i + 1) % 4][(j + 2) % 4] * matrix[(i + 2) % 4][(j + 3) % 4] * matrix[(i + 3) % 4][(j + 1) % 4] +
                                                              matrix[(i + 1) % 4][(j + 3) % 4] * matrix[(i + 2) % 4][(j + 1) % 4] * matrix[(i + 3) % 4][(j + 2) % 4] -
                                                              matrix[(i + 1) % 4][(j + 3) % 4] * matrix[(i + 2) % 4][(j + 2) % 4] * matrix[(i + 3) % 4][(j + 1) % 4] -
                                                              matrix[(i + 1) % 4][(j + 2) % 4] * matrix[(i + 2) % 4][(j + 1) % 4] * matrix[(i + 3) % 4][(j + 3) % 4] -
                                                              matrix[(i + 1) % 4][(j + 1) % 4] * matrix[(i + 2) % 4][(j + 3) % 4] * matrix[(i + 3) % 4][(j + 2) % 4]);
        }
    }

    return cofactorMatrix;
}

Matrix Matrix::adjugate() const
{
    return this->cofactor().transpose();
}

Matrix Matrix::inverse() const
{
    return this->adjugate() / this->determinant();
}

// Operator overloading

Matrix Matrix::operator+(const Matrix& matrix) const
{
    Matrix sumMatrix;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            sumMatrix.matrix[i][j] = this->matrix[i][j] + matrix.matrix[i][j];
        }
    }

    return sumMatrix;
}

Matrix Matrix::operator-(const Matrix& matrix) const
{
    Matrix differenceMatrix;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            differenceMatrix.matrix[i][j] = this->matrix[i][j] - matrix.matrix[i][j];
        }
    }

    return differenceMatrix;
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
    Matrix productMatrix;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            for (int k = 0; k < MATRIX_SIZE; k++)
            {
                productMatrix.matrix[i][j] += this->matrix[i][k] * matrix.matrix[k][j];
            }
        }
    }

    return productMatrix;
}

Vector3D Matrix::operator*(const Vector3D& vector) const
{
   double colMat[4];

   for(int i = 0; i < MATRIX_SIZE; i++)
   {
       colMat[i] = 0;

       for(int j = 0; j < MATRIX_SIZE; j++)
       {
           colMat[i] += this->matrix[i][j] * (j == 0 ? vector.getX() : (j == 1 ? vector.getY() : (j == 2 ? vector.getZ() : vector.getW())));
       }
   }

    Vector3D productVector(colMat[0], colMat[1], colMat[2], colMat[3]);

    return productVector;
}

Matrix Matrix::operator*(double scalar) const
{
    Matrix productMatrix;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            productMatrix.matrix[i][j] = this->matrix[i][j] * scalar;
        }
    }

    return productMatrix;
}

Matrix Matrix::operator/(double scalar) const
{
    Matrix quotientMatrix;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            quotientMatrix.matrix[i][j] = this->matrix[i][j] / scalar;
        }
    }

    return quotientMatrix;
}

// Assignment operator

Matrix& Matrix::operator=(const Matrix& matrix)
{
    if (this == &matrix)
    {
        return *this;
    }

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            this->matrix[i][j] = matrix.matrix[i][j];
        }
    }

    return *this;
}

// Equality operator

bool Matrix::operator==(const Matrix& matrix) const
{
    bool equal = true;

    for (int i = 0; i < MATRIX_SIZE && equal; i++)
    {
        for (int j = 0; j < MATRIX_SIZE && equal; j++)
        {
            if (this->matrix[i][j] != matrix.matrix[i][j])
            {
                equal = false;
            }
        }
    }

    return equal;
}

// Stream insertion and extraction

std::ostream& operator<<(std::ostream& stream, const Matrix& matrix)
{
    stream << std::endl;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        stream << "| ";

        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            stream << matrix.matrix[i][j] << " ";
        }

        stream << "|" << std::endl;
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, Matrix& matrix)
{
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            stream >> matrix.matrix[i][j];
        }
    }

    return stream;
}

