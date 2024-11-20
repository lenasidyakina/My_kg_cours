
#ifndef CURS_CUBE_H
#define CURS_CUBE_H

#include <QMetaType>
#include <qpainter.h>

#include "Figure.h"
#include "Sphere.h"

struct Point3D {
    float x, y, z;

    Point3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};
class Cube : public Figure {
public:
    Cube(QString color, int edgeLength, float theta, float phi) : Figure(color) {
        this->EdgeLength = edgeLength;
        x = 0; y = 0; z = 0;
        phi = 0;
        theta = 0;
    }
    int EdgeLength;
    float theta = 0;
    float phi = 0;
    Point3D position;
    virtual void Draw() override;
    virtual QString getName() override;
    virtual int GetType() override;

    void Draw(QPainter *painter, Cube *cube, Sphere *sphere);

};

Q_DECLARE_METATYPE( Cube )

#endif //CURS_CUBE_H
