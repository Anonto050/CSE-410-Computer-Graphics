#include "matrix.h"
#include <cmath>

#define PI 2.0 * acos(0.0)

class transformation
{
    private:
        Matrix Tmat;

    public:

        transformation();
        transformation(const Matrix& matrix) : Tmat(matrix) {}
        ~transformation();

        //Convert transformation to matrix
        Matrix toMatrix() const { return Tmat; }

        // Getters and setters
        double getMatrixValue(int row, int column) const;
        void setMatrixValue(int row, int column, double value);

        Vector3D applyRotationFormula(Vector3D vector, Vector3D axis, double angle);

        transformation translation(double tx, double ty, double tz);
        transformation scale(double sx, double sy, double sz);
        transformation rotation(double angle, double rx, double ry, double rz);
        transformation viewTransformation(Vector3D eye, Vector3D gaze, Vector3D up);
        transformation projectionTransformation(double fovY, double aspectRatio, double near, double far);

        // Operator overloading
        transformation operator*(const transformation& tf) const;
        Vector3D operator*(const Vector3D& vector) const;

        // Assignment operator
        transformation& operator=(const transformation& tf)
        {
            Tmat = tf.Tmat;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const transformation& tf)
        {
            os << tf.Tmat;
            return os;
        }

};

// Constructors and destructor
transformation::transformation() 
{
    Tmat = Matrix();
    Tmat = Tmat.createIdentityMatrix();
}

transformation::~transformation() {}

// Getters and setters
double transformation::getMatrixValue(int row, int column) const
{
    return Tmat.getMatrixValue(row, column);
}

void transformation::setMatrixValue(int row, int column, double value)
{
    Tmat.setMatrixValue(row, column, value);
}

Vector3D transformation::applyRotationFormula(Vector3D vector, Vector3D axis, double angle)
{
    double cosAngle = cos(angle * PI / 180.0);
    double sinAngle = sin(angle * PI / 180.0);

    return vector * cosAngle + axis.crossProduct(vector) * sinAngle + axis * axis.dotProduct(vector) * (1 - cosAngle);
}

transformation transformation::translation(double tx, double ty, double tz)
{
    transformation translationMatrix;

    translationMatrix.setMatrixValue(0, 3, tx);
    translationMatrix.setMatrixValue(1, 3, ty);
    translationMatrix.setMatrixValue(2, 3, tz);

    return translationMatrix;
}

transformation transformation::scale(double sx, double sy, double sz)
{
    transformation scaleMatrix;

    scaleMatrix.setMatrixValue(0, 0, sx);
    scaleMatrix.setMatrixValue(1, 1, sy);
    scaleMatrix.setMatrixValue(2, 2, sz);

    return scaleMatrix;
}

transformation transformation::rotation(double angle, double rx, double ry, double rz)
{
    transformation rotationMatrix;

    Vector3D axis = Vector3D(rx, ry, rz).normalize();

    rotationMatrix.setMatrixValue(0, 0, applyRotationFormula(Vector3D(1.0, 0.0, 0.0), axis, angle).getX());
    rotationMatrix.setMatrixValue(1, 0, applyRotationFormula(Vector3D(1.0, 0.0, 0.0), axis, angle).getY());
    rotationMatrix.setMatrixValue(2, 0, applyRotationFormula(Vector3D(1.0, 0.0, 0.0), axis, angle).getZ());

    rotationMatrix.setMatrixValue(0, 1, applyRotationFormula(Vector3D(0.0, 1.0, 0.0), axis, angle).getX());
    rotationMatrix.setMatrixValue(1, 1, applyRotationFormula(Vector3D(0.0, 1.0, 0.0), axis, angle).getY());
    rotationMatrix.setMatrixValue(2, 1, applyRotationFormula(Vector3D(0.0, 1.0, 0.0), axis, angle).getZ());

    rotationMatrix.setMatrixValue(0, 2, applyRotationFormula(Vector3D(0.0, 0.0, 1.0), axis, angle).getX());
    rotationMatrix.setMatrixValue(1, 2, applyRotationFormula(Vector3D(0.0, 0.0, 1.0), axis, angle).getY());
    rotationMatrix.setMatrixValue(2, 2, applyRotationFormula(Vector3D(0.0, 0.0, 1.0), axis, angle).getZ());

    return rotationMatrix;
}

transformation transformation::viewTransformation(Vector3D eye, Vector3D look, Vector3D up)
{
    transformation rotationMatrix;

    Vector3D l = (look - eye).normalize();
    Vector3D r = l.crossProduct(up).normalize();
    Vector3D u = r.crossProduct(l);

    rotationMatrix.setMatrixValue(0, 0, r.getX());
    rotationMatrix.setMatrixValue(0, 1, r.getY());
    rotationMatrix.setMatrixValue(0, 2, r.getZ());

    rotationMatrix.setMatrixValue(1, 0, u.getX());
    rotationMatrix.setMatrixValue(1, 1, u.getY());
    rotationMatrix.setMatrixValue(1, 2, u.getZ());

    rotationMatrix.setMatrixValue(2, 0, -l.getX());
    rotationMatrix.setMatrixValue(2, 1, -l.getY());
    rotationMatrix.setMatrixValue(2, 2, -l.getZ());

    transformation translationMatrix = translation(-eye.getX(), -eye.getY(), -eye.getZ());

    return rotationMatrix * translationMatrix;
}



transformation transformation::projectionTransformation(double fovY, double aspectRatio, double near, double far)
{
    transformation projectionMatrix;

    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY * PI / 360.0);
    double r = near * tan(fovX * PI / 360.0);

    projectionMatrix.setMatrixValue(0, 0, near / r);
    projectionMatrix.setMatrixValue(1, 1, near / t);
    projectionMatrix.setMatrixValue(2, 2, -(far + near) / (far - near));
    projectionMatrix.setMatrixValue(2, 3, -(2.0 * far * near) / (far - near));
    projectionMatrix.setMatrixValue(3, 2, -1.0);
    projectionMatrix.setMatrixValue(3, 3, 0.0);

    return projectionMatrix;
}

// Operator overloading

transformation transformation::operator*(const transformation& tf) const
{
    transformation result;

    result.Tmat = Tmat * tf.Tmat;

    return result;
}

Vector3D transformation::operator*(const Vector3D& vector) const
{
    Vector3D result;

    result = Tmat * vector;

    return result;
}


