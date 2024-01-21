#include "transformation.cpp"
#include "vector3D.cpp"
#include "matrix.cpp"
#include <stack>
#include <fstream>
#include <string>

#include "bitmap_image.hpp"

using namespace std;

int leftMostIndex = 0;
int rightMostIndex = 0;
Vector3D leftMostPoint;
Vector3D rightMostPoint;

// Random function
static unsigned long int g_seed = 1;

inline int random()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

// function for comparing two doubles with a tolerance of 1 pixel
bool isDivisiblebyZero(double a, double b, double tolerance)
{
    if (fabs(a - b) < tolerance)
    {
        return true;
    }
    else
    {
        return false;
    }
}



// Struct to store the color of a pixel and typedef it to Color

struct Color
{
    int RED;
    int GREEN;
    int BLUE;
};

typedef struct Color color;

class Triangle
{
private:
    // Vertices of the triangle
    Vector3D v1;
    Vector3D v2;
    Vector3D v3;

    // Color of the triangle
    color c;

public:
    Triangle();
    Triangle(const Vector3D &vertex1, const Vector3D &vertex2, const Vector3D &vertex3)
        : v1(vertex1), v2(vertex2), v3(vertex3), c(Color{0, 0, 0}) {}
    ~Triangle();

    // Getters and setters
    Vector3D getV1() const { return v1; }
    Vector3D getV2() const { return v2; }
    Vector3D getV3() const { return v3; }
    color getColor() const { return c; }

    // Get the vertices of the triangle
    Vector3D getVertex(int index) const;

    void setV1(Vector3D v) { v1 = v; }
    void setV2(Vector3D v) { v2 = v; }
    void setV3(Vector3D v) { v3 = v; }
    void setColor(color c) { this->c = c; }

    // Operator overloading
    Triangle operator*(const transformation &tf) const;

    // Assignment operator
    Triangle &operator=(const Triangle &triangle);

    friend istream &operator>>(istream &is, Triangle &triangle);
    friend ostream &operator<<(ostream &os, const Triangle &triangle);
};

Triangle::Triangle()
{
    v1 = Vector3D();
    v2 = Vector3D();
    v3 = Vector3D();

    c.RED = 0;
    c.GREEN = 0;
    c.BLUE = 0;
}

Triangle::~Triangle() {}

Vector3D Triangle::getVertex(int index) const
{
    if (index == 0)
    {
        return v1;
    }
    else if (index == 1)
    {
        return v2;
    }
    else if (index == 2)
    {
        return v3;
    }
    else
    {
        cout << "Error: Invalid index." << endl;
        return Vector3D();
    }
}

Triangle Triangle::operator*(const transformation &tf) const
{
    Vector3D newV1 = tf * v1;
    Vector3D newV2 = tf * v2;
    Vector3D newV3 = tf * v3;

    // Divide by w
    newV1 = newV1.scale();
    newV2 = newV2.scale();
    newV3 = newV3.scale();

    return Triangle(newV1, newV2, newV3);
}

Triangle &Triangle::operator=(const Triangle &triangle)
{
    v1 = triangle.v1;
    v2 = triangle.v2;
    v3 = triangle.v3;
    return *this;
}

istream &operator>>(istream &is, Triangle &triangle)
{
    is >> triangle.v1 >> triangle.v2 >> triangle.v3;
    return is;
}

ostream &operator<<(ostream &os, const Triangle &triangle)
{
    os << triangle.v1 << "\n"
       << triangle.v2 << "\n"
       << triangle.v3 << "\n";
    return os;
}

// Functions to get top scanline and bottom scanline

int getTopScanline(Triangle triangle, double TopY, double dy)
{
    int topScanline = 0;
    // Find max y coordinate among the vertices
    double maxY = max(triangle.getV1().getY(), max(triangle.getV2().getY(), triangle.getV3().getY()));

    // Compare the max y coordinate with the given TopY
    if (maxY < TopY)
    {
        topScanline = int(round((TopY - maxY) / dy));
    }
    else
    {
        topScanline = 0;
    }

    return topScanline;
}

int getBottomScanline(Triangle triangle, double BottomY, double dy, int screenHeight)
{
    int bottomScanline = 0;
    // Find min y coordinate among the vertices
    double minY = min(triangle.getV1().getY(), min(triangle.getV2().getY(), triangle.getV3().getY()));

    // Compare the min y coordinate with the given BottomY
    if (minY > BottomY)
    {
        bottomScanline = screenHeight - 1 - int(round((minY - BottomY) / dy));
    }
    else
    {
        bottomScanline = screenHeight - 1;
    }

    return bottomScanline;
}

// Functions to get left clipLine and right clipLine
int getLeftClipLine(int numIntersectionPoints, Vector3D intersectionPoints[], double left_X, double dx)
{
    int leftClipLine = 0;

    // Find the left most intersection point
    leftMostPoint = Vector3D(INFINITY, 0, 0);

    for (int i = 0; i < numIntersectionPoints; i++)
    {
        if (intersectionPoints[i].getX() != INFINITY)
        {
            if (intersectionPoints[i].getX() < leftMostPoint.getX())
            {
                leftMostPoint = intersectionPoints[i];
                leftMostIndex = i;
            }
        }
    }

    // Compare the x coordinate of the left most intersection point with the given left_X
    if (leftMostPoint.getX() <= left_X)
    {
        leftClipLine = 0;
    }
    else
    {
        leftClipLine = int(round((leftMostPoint.getX() - left_X) / dx));
    }

    if(leftMostPoint.getX() == INFINITY)
    {
        leftClipLine = 0;
    }

    cout << "leftMostPoint: " << leftMostPoint << endl;

    return leftClipLine;
}

int getRightClipLine(int numIntersectionPoints, Vector3D intersectionPoints[], double right_X, double dx, int screenWidth)
{
    int rightClipLine = 0;

    // Find the right most intersection point
    rightMostPoint = Vector3D(-INFINITY, 0, 0);

    for (int i = 0; i < numIntersectionPoints; i++)
    {
        if (intersectionPoints[i].getX() != INFINITY)
        {
            if (intersectionPoints[i].getX() > rightMostPoint.getX())
            {
                rightMostPoint = intersectionPoints[i];
                rightMostIndex = i;
            }
        }
    }

    // Compare the x coordinate of the right most intersection point with the given right_X
    if (rightMostPoint.getX() >= right_X)
    {
        rightClipLine = screenWidth - 1;
    }
    else
    {
        rightClipLine = screenWidth - 1 - int(round((right_X - rightMostPoint.getX()) / dx));
    }

    if(rightMostPoint.getX() == -INFINITY)
    {
        rightClipLine = 0;
    }

    cout << "rightMostPoint: " << rightMostPoint << endl;

    return rightClipLine;
}

int main(int argc, char *argv[])
{
    ifstream inputFile;
    ofstream outputFile;

    string inputFileSerial = "4";
    string inputFileName = "./tests/" + inputFileSerial + "/scene.txt";

    inputFile.open(inputFileName.c_str());

    if (!inputFile.is_open())
    {
        cout << "Error opening input file." << endl;
        return 1;
    }

    string outputFileName = "./tests/" + inputFileSerial + "/stage1.txt";

    outputFile.open(outputFileName.c_str());

    if (!outputFile.is_open())
    {
        cout << "Error opening output file." << endl;
        return 1;
    }

    int numTriangles = 0;
    int numPushes = 0;

    // Initialize eye, look and up vectors and fovY, aspectRatio, near and far values
    Vector3D eye;
    Vector3D look;
    Vector3D up;
    double fovY, aspectRatio, near, far;

    // Read eye, look and up vectors and fovY, aspectRatio, near and far values
    inputFile >> eye;
    inputFile >> look;
    inputFile >> up;

    inputFile >> fovY;
    inputFile >> aspectRatio;
    inputFile >> near;
    inputFile >> far;

    // Initialize transformation stack
    stack<transformation> transformationStack;

    // Initialize transformation matrices
    transformation identityMatrix;
    transformationStack.push(identityMatrix);

    string command;

    // Stage 1 - Modeling transformations

    while (true)
    {
        inputFile >> command;

        if (command == "triangle")
        {
            Triangle triangle;

            inputFile >> triangle;

            triangle = triangle * transformationStack.top();

            outputFile << triangle << endl;

            numTriangles++;
        }
        else if (command == "translate")
        {
            double tx, ty, tz;

            inputFile >> tx;
            inputFile >> ty;
            inputFile >> tz;

            transformation translationMatrix;
            translationMatrix = translationMatrix.translation(tx, ty, tz);

            transformation t = transformationStack.top() * translationMatrix;

            transformationStack.pop();
            transformationStack.push(t);
        }
        else if (command == "scale")
        {
            double sx, sy, sz;

            inputFile >> sx;
            inputFile >> sy;
            inputFile >> sz;

            transformation scaleMatrix;
            scaleMatrix = scaleMatrix.scale(sx, sy, sz);

            transformation t = transformationStack.top() * scaleMatrix;

            transformationStack.pop();
            transformationStack.push(t);
        }
        else if (command == "rotate")
        {
            double angle, rx, ry, rz;

            inputFile >> angle;
            inputFile >> rx;
            inputFile >> ry;
            inputFile >> rz;

            transformation rotationMatrix;
            rotationMatrix = rotationMatrix.rotation(angle, rx, ry, rz);

            transformation t = transformationStack.top() * rotationMatrix;

            transformationStack.pop();
            transformationStack.push(t);
        }
        else if (command == "push")
        {
            transformationStack.push(transformationStack.top());

            numPushes++;
        }
        else if (command == "pop")
        {
            // If there are no more matrices to pop, do nothing
            if (numPushes == 0)
            {
                cout << "Error: There are no more matrices to pop." << endl;
                break;
            }

            transformationStack.pop();

            numPushes--;
        }
        else if (command == "end")
        {
            break;
        }
        else
        {
            cout << "Error: Invalid command." << endl;
            break;
        }
    }

    inputFile.close();
    outputFile.close();

    // Stage 2 - Viewing transformation

    inputFile.open(outputFileName.c_str());

    if (!inputFile.is_open())
    {
        cout << "Error opening input file." << endl;
        return 1;
    }

    outputFileName = "./tests/" + inputFileSerial + "/stage2.txt";

    outputFile.open(outputFileName.c_str());

    if (!outputFile.is_open())
    {
        cout << "Error opening output file." << endl;
        return 1;
    }

    // Iterate through the triangles and apply the viewing transformation
    for (int i = 0; i < numTriangles; i++)
    {
        Triangle triangle;

        inputFile >> triangle;

        // Apply the viewing transformation
        transformation viewMatrix;
        viewMatrix = viewMatrix.viewTransformation(eye, look, up);

        triangle = triangle * viewMatrix;

        outputFile << triangle << endl;
    }

    inputFile.close();
    outputFile.close();

    // Stage 3 - Projection transformation

    inputFile.open(outputFileName.c_str());

    if (!inputFile.is_open())
    {
        cout << "Error opening input file." << endl;
        return 1;
    }

    outputFileName = "./tests/" + inputFileSerial + "/stage3.txt";

    outputFile.open(outputFileName.c_str());

    if (!outputFile.is_open())
    {
        cout << "Error opening output file." << endl;
        return 1;
    }

    // Iterate through the triangles and apply the projection transformation

    for (int i = 0; i < numTriangles; i++)
    {
        Triangle triangle;

        inputFile >> triangle;

        // Apply the projection transformation
        transformation projectionMatrix;
        projectionMatrix = projectionMatrix.projectionTransformation(fovY, aspectRatio, near, far);

        triangle = triangle * projectionMatrix;

        outputFile << triangle << endl;
    }

    inputFile.close();
    outputFile.close();

    // Stage 4 - clipping and scan conversion using z-buffer algorithm

    // Read screen width and height from config.txt
    int screenWidth, screenHeight;

    double pixelSize = 2.0 / 500;

    string configFileName = "./tests/" + inputFileSerial + "/config.txt";

    inputFile.open(configFileName.c_str());

    if (!inputFile.is_open())
    {
        cout << "Error opening input file." << endl;
        return 1;
    }

    inputFile >> screenWidth;
    inputFile >> screenHeight;

    inputFile.close();

    // Read triangles from stage3.txt
    inputFile.open(outputFileName.c_str());

    if (!inputFile.is_open())
    {
        cout << "Error opening input file." << endl;
        return 1;
    }

    // Take the triangles in a vector
    vector<Triangle> triangles;

    for (int i = 0; i < numTriangles; i++)
    {
        Triangle triangle;

        inputFile >> triangle;

        // Add color to the triangle
        int r = random() % 256;
        int g = random() % 256;
        int b = random() % 256;

        triangle.setColor(Color{r, g, b});

        triangles.push_back(triangle);
    }

    // Output file for stage 4
    outputFileName = "./tests/" + inputFileSerial + "/z_buffer.txt";

    outputFile.open(outputFileName.c_str());

    if (!outputFile.is_open())
    {
        cout << "Error opening output file." << endl;
        return 1;
    }

    // Define constants for the z-buffer algorithm
    double rightLimit = 1;
    double leftLimit = -1;
    double topLimit = 1;
    double bottomLimit = -1;
    double zMin = -1;
    double zMax = 1;

    // Pixel mapping between x-y range and screen width-height

    double dx, dy, top_Y, bottom_Y, left_X, right_X;

    dx = (rightLimit - leftLimit) / screenWidth;
    dy = (topLimit - bottomLimit) / screenHeight;

    top_Y = topLimit - dy / 2;
    bottom_Y = bottomLimit + dy / 2;

    left_X = leftLimit + dx / 2;
    right_X = rightLimit - dx / 2;

    // Initialize z-buffer
    double **zBuffer = new double *[screenHeight];

    for (int i = 0; i < screenHeight; i++)
    {
        zBuffer[i] = new double[screenWidth];
    }

    // Initialize color buffer
    color **colorBuffer = new color *[screenHeight];

    for (int i = 0; i < screenHeight; i++)
    {
        colorBuffer[i] = new color[screenWidth];
    }

    // Initialize z-buffer and color buffer
    for (int i = 0; i < screenHeight; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            zBuffer[i][j] = zMax;
            colorBuffer[i][j] = Color{0, 0, 0};
        }
    }

    // Create a bitmap image with the given screen width and height and initialize it to black
    bitmap_image image(screenWidth, screenHeight);

    image.set_all_channels(0, 0, 0);

    // Apply the z-buffer algorithm
    for (int i = 0; i < numTriangles; i++)
    {
        Triangle triangle = triangles[i];

        // Get the top and bottom scanlines
        int topScanline = getTopScanline(triangle, top_Y, dy);
        int bottomScanline = getBottomScanline(triangle, bottom_Y, dy, screenHeight);

        // Iterate through the scanlines
        for (int scanline = topScanline; scanline <= bottomScanline; scanline++)
        {
            // Get the y coordinate of the scanline
            double y = top_Y - scanline * dy;

            // Find the intersection points of the scanline with the triangle
            const int numIntersectionPoints = 3;
            Vector3D intersectionPoints[numIntersectionPoints];

            // Initialize the intersection points with x being infinity, y being the y coordinate of the scanline and z being 0
            for (int i = 0; i < numIntersectionPoints; i++)
            {
                intersectionPoints[i] = Vector3D(INFINITY, y, 0);
            }

            // Set the x coordinate of the intersection points
            for (int i = 0; i < numIntersectionPoints; i++)
            {
                // Take a pair of vertices of an edge
                Vector3D v1 = triangle.getVertex((numIntersectionPoints - i) % numIntersectionPoints);
                Vector3D v2 = triangle.getVertex((numIntersectionPoints - i - 1) % numIntersectionPoints);

                // If the edge is horizontal, skip it

                if (isDivisiblebyZero(v1.getY(), v2.getY(), pixelSize))
                {
                    continue;
                }

                else
                {
                    // set the x coordinate of the intersection point
                    double x = v1.getX() + (y - v1.getY()) * (v2.getX() - v1.getX()) / (v2.getY() - v1.getY());

                    intersectionPoints[i].setX(x);
                }
            }

            // Filter out the intersection points that are outside the x range and y range
            for (int i = 0; i < numIntersectionPoints; i++)
            {
                Vector3D v1 = triangle.getVertex((numIntersectionPoints - i) % numIntersectionPoints);
                Vector3D v2 = triangle.getVertex((numIntersectionPoints - i - 1) % numIntersectionPoints);

                if (intersectionPoints[i].getX() != INFINITY)
                {
                    if (intersectionPoints[i].getX() < min(v1.getX(), v2.getX()) || intersectionPoints[i].getX() > max(v1.getX(), v2.getX()))
                    {
                        intersectionPoints[i].setX(INFINITY);
                    }
                    if (intersectionPoints[i].getY() < min(v1.getY(), v2.getY()) || intersectionPoints[i].getY() > max(v1.getY(), v2.getY()))
                    {
                        intersectionPoints[i].setX(INFINITY);
                    }
                }
            }

            // Find leftClip and rightClip after clipping the triangle
            int leftClip = getLeftClipLine(numIntersectionPoints, intersectionPoints, left_X, dx);
            int rightClip = getRightClipLine(numIntersectionPoints, intersectionPoints, right_X, dx, screenWidth);

            // Calculate z_a and z_b values
            double z_a = 0;
            double z_b = 0;

            Vector3D v1 = triangle.getVertex((numIntersectionPoints - leftMostIndex) % numIntersectionPoints);
            Vector3D v2 = triangle.getVertex((numIntersectionPoints - leftMostIndex - 1) % numIntersectionPoints);

            if (!isDivisiblebyZero(v1.getY(), v2.getY(), pixelSize))
            {
                z_a = v1.getZ() + (y - v1.getY()) * (v2.getZ() - v1.getZ()) / (v2.getY() - v1.getY());
            }

            v1 = triangle.getVertex((numIntersectionPoints - rightMostIndex) % numIntersectionPoints);
            v2 = triangle.getVertex((numIntersectionPoints - rightMostIndex - 1) % numIntersectionPoints);

            if (!isDivisiblebyZero(v1.getY(), v2.getY(), pixelSize))
            {
                z_b = v1.getZ() + (y - v1.getY()) * (v2.getZ() - v1.getZ()) / (v2.getY() - v1.getY());
            }

            // Calculate dz_dx
            double constant = 0;

            // Calculate z value for each pixel
            double z_p = 0;
            double x_a = leftMostPoint.getX();
            double x_b = rightMostPoint.getX();

            constant = dx * (z_b - z_a) / (x_b - x_a);

            for (int col = leftClip; col <= rightClip; col++)
            {
                // Calculate z_p for first pixel
                if (col == leftClip)
                {
                    double x_p = left_X + col * dx;

                    z_p = z_a + (x_p - x_a) * (z_b - z_a) / (x_b - x_a);
                }
                else
                {
                    z_p = z_p + constant;
                }

                // If z_p is less than zMin or greater than zMax, skip the pixel
                if (z_p < zMin || z_p > zMax)
                {
                    continue;
                }

                // If z_p is less than zBuffer[scanline][col], update zBuffer[scanline][col] and colorBuffer[scanline][col]
                if (z_p < zBuffer[scanline][col])
                {
                    zBuffer[scanline][col] = z_p;

                    int r = triangle.getColor().RED;
                    int g = triangle.getColor().GREEN;
                    int b = triangle.getColor().BLUE;

                    colorBuffer[scanline][col] = Color{r, g, b};
                }

                cout << scanline<< "  " << col << endl;
            }
        }
    }

    // Write the color buffer to the image
    for (int i = 0; i < screenHeight; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            image.set_pixel(j, i, colorBuffer[i][j].RED, colorBuffer[i][j].GREEN, colorBuffer[i][j].BLUE);
        }
    }

    // Save the image
    image.save_image("./tests/" + inputFileSerial + "/out.bmp");

    // Output the z-buffer to the output file
    for (int i = 0; i < screenHeight; i++)
    {
        for (int j = 0; j < screenWidth; j++)
        {
            if (zBuffer[i][j] < zMax)
            {
                outputFile << setprecision(6) << fixed << round(zBuffer[i][j] * 1000000) / 1000000 << "\t";
            }
        }
        outputFile << endl;
    }

    inputFile.close();
    outputFile.close();

    // Free memory
    for (int i = 0; i < screenHeight; i++)
    {
        delete[] zBuffer[i];
    }

    delete[] zBuffer;

    for (int i = 0; i < screenHeight; i++)
    {
        delete[] colorBuffer[i];
    }

    delete[] colorBuffer;

    return 0;
}
