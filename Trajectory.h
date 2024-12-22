
#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <QMetaType>
#include <QVector3D>
#include <qpainter.h>

#include "CurTypes.h"


class Trajectory {
public:
    Trajectory(QString color, int phi, int theta) {
        this->phi = phi;
        this->theta = theta;
        this->color = ColorToInt(color);
    }
    int color = 958;
    int phi = 0;
    int theta = 0;
    int step = 0;
    static uint ColorToInt(const QString color)
    {
        bool ok;
        uint c =  color.mid(1).toUInt(&ok, 16);
        if (!ok)
            c = QString(DEFAULT_FIGURE_COLOR).mid(1).toUInt(&ok, 16);
        return c;
    }
    QVector3D point3(int r) {

        float phiRad = phi * M_PI / 180.0f;  // Угол наклона плоскости (phi)
        float thetaRad = theta * M_PI / 180.0f;  // Угол отклонения (theta)

        float angle = step * 2 * M_PI / 360;

        // Координаты эллипса в локальной системе
        float x = (100 + r) * cos(angle);
        float y = (100 + r) * sin(angle);

        // Поворот плоскости на угол phi вокруг оси X
        float yRotPhi = y * cos(phiRad);
        float zRotPhi = y * sin(phiRad);

        // Поворот на угол theta вокруг оси Z
        float xRot = x * cos(thetaRad) - zRotPhi * sin(thetaRad);
        float yRot = x * sin(thetaRad) + zRotPhi * cos(thetaRad);
        float zRot = yRotPhi;  // Z после поворота вокруг Z

        return QVector3D( xRot, yRot, zRot );
    }

};

Q_DECLARE_METATYPE( Trajectory )

#endif //TRAJECTORY_H
