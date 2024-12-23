
#ifndef SPHERE_H
#define SPHERE_H


#include <QMetaType>
#include <qpainter.h>

#include "Figure.h"
#include "Trajectory.h"


class Sphere : public Figure {
public:
    Sphere(QString color, int radiusLength) : Figure(color) {
        this->RadiusLength = radiusLength;
        x = 0; y = 0; z = 0;
    }
    int RadiusLength;
    virtual void Draw() override;
    virtual QString getName() override;
    virtual int GetType() override;
    int slices = DEFAULT_SLICES; // Количество секторов по горизонтали
    int stacks = DEFAULT_STACKS; // Количество секторов по вертикали



    void Draw(QPainter *painter, Sphere *base_sphere, Sphere *sphere, Trajectory *trajectory, std::vector<Poly> &polys);

    void DrawGlobe(QPainter *painter, Sphere *sphere, std::vector<Poly> &polys);

    void DrawGlobe(QPainter *painter, Sphere *sphere);

    QVector3D CalculateVertex3D(const QVector3D &contactPoint, float radius, double theta, double phi);

    void Draw(QPainter *painter, Sphere *base_sphere, Sphere *sphere, Trajectory *trajectory);
    QVector3D center; // Центр тетраэдра

    QVector3D getPosition() const {
        return center; // Возвращаем центр тетраэдра
    }
};

Q_DECLARE_METATYPE( Sphere )

#endif //SPHERE_H
