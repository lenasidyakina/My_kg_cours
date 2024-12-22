
#include "Sphere.h"


#include <QPainter>
#include <QVector3D>

#include "CurTypes.h"

void Sphere::Draw()
{

}


QString Sphere::getName()
{
    return "Сфера " + QString("%1px").arg(this->RadiusLength);
}
QVector3D Sphere::CalculateVertex3D(const QVector3D &contactPoint, float radius, double theta, double phi) {
    QVector3D vertex;
    vertex.setX(contactPoint.x() + radius * sin(theta) * cos(phi));
    vertex.setY(contactPoint.y() + radius * sin(theta) * sin(phi));
    vertex.setZ(contactPoint.z() + radius * cos(theta));
    return vertex;
}

int Sphere::GetType() { return FIGURE_SPHERE; }



