
#ifndef CURS_CUBE_H
#define CURS_CUBE_H

#include <QMetaType>
#include <qpainter.h>
#include <QVector3D>

#include "Figure.h"
#include "Sphere.h"
#include "Trajectory.h"

struct Point3D {
    float x, y, z;
    Point3D(const QVector3D& vec) : x(vec.x()), y(vec.y()), z(vec.z()) {}

    Point3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Point3D operator*(float scalar) const {
        return Point3D(x * scalar, y * scalar, z * scalar);
    }

    // Перегрузка оператора сложения
    Point3D operator+(const Point3D& other) const {
        return Point3D(x + other.x, y + other.y, z + other.z);
    }

    // Перегрузка оператора вычитания
    Point3D operator-(const Point3D& other) const {
        return Point3D(x - other.x, y - other.y, z - other.z);
    }
};
class Cube : public Figure {
public:
    Cube(QString color, int edgeLength, float theta, float phi) : Figure(color) {
        this->EdgeLength = edgeLength;
        x = 0; y = 0; z = 0;
        phi = 0;
        theta = 0;
    }
    std::vector<Point3D> vertices;
    int EdgeLength;
    float theta = 0;
    float phi = 0;
    QVector3D center;
    Point3D position = (0,0,0);
    virtual void Draw() override;
    virtual QString getName() override;

    void updateVertices(const Point3D &center, const Point3D &tangent, const Point3D &up, const Point3D &normal);

    virtual int GetType() override;

    void Draw(QPainter *painter, Cube *cube, Sphere *sphere, std::vector<Poly> &polys);

};

Q_DECLARE_METATYPE( Cube )

#endif //CURS_CUBE_H
