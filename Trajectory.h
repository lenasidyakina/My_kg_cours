//
// Created by sidya on 21.11.2024.
//

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <QMetaType>
#include <qpainter.h>

#include "CurTypes.h"
#include <QVector3D>
#include <QVector2D>


#define TRAJECTORY_STEP 1
#define TRAJECTORY_MAX_STEPS 360
#define GLOBE_RADIUS 100 // todo:

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

    QVector3D point3(float dx, float dy, float dz) {
        float phiRad = phi * M_PI / 180.0f;  // Угол наклона плоскости (phi)
        float thetaRad = theta * M_PI / 180.0f;  // Угол отклонения (theta)

        float angle = step * 2 * M_PI / TRAJECTORY_MAX_STEPS;

        // Координаты эллипса в локальной системе
        float x = (GLOBE_RADIUS + dx) * cos(angle);
        float y = (GLOBE_RADIUS + dy) * sin(angle);

        // Поворот плоскости на угол phi вокруг оси X
        float yRotPhi = y * cos(phiRad);
        float zRotPhi = y * sin(phiRad);

        // Поворот на угол theta вокруг оси Z
        float xRot = x * cos(thetaRad) - zRotPhi * sin(thetaRad);
        float yRot = x * sin(thetaRad) + zRotPhi * cos(thetaRad);
        float zRot = yRotPhi;  // Z после поворота вокруг Z

        return QVector3D( xRot, yRot, zRot );
    }

    QVector3D point3(int dr, int ds) {

        float phiRad = phi * M_PI / 180.0f;  // Угол наклона плоскости (phi)
        float thetaRad = theta * M_PI / 180.0f;  // Угол отклонения (theta)

        float angle = (step + ds) * 2 * M_PI / TRAJECTORY_MAX_STEPS;

        // Координаты эллипса в локальной системе
        float x = (GLOBE_RADIUS + dr) * cos(angle);
        float y = (GLOBE_RADIUS + dr) * sin(angle);

        // Поворот плоскости на угол phi вокруг оси X
        float yRotPhi = y * cos(phiRad);
        float zRotPhi = y * sin(phiRad);

        // Поворот на угол theta вокруг оси Z
        float xRot = x * cos(thetaRad) - zRotPhi * sin(thetaRad);
        float yRot = x * sin(thetaRad) + zRotPhi * cos(thetaRad);
        float zRot = yRotPhi;  // Z после поворота вокруг Z

        return QVector3D( xRot, yRot, zRot );
    }

    QVector3D point(int dr, int pos) {

        float phiRad = phi * M_PI / 180.0f;  // Угол наклона плоскости (phi)
        float thetaRad = theta * M_PI / 180.0f;  // Угол отклонения (theta)

        float angle = (pos) * 2 * M_PI / TRAJECTORY_MAX_STEPS;

        // Координаты эллипса в локальной системе
        float x = (GLOBE_RADIUS + dr) * cos(angle);
        float y = (GLOBE_RADIUS + dr) * sin(angle);

        // Поворот плоскости на угол phi вокруг оси X
        float yRotPhi = y * cos(phiRad);
        float zRotPhi = y * sin(phiRad);

        // Поворот на угол theta вокруг оси Z
        float xRot = x * cos(thetaRad) - zRotPhi * sin(thetaRad);
        float yRot = x * sin(thetaRad) + zRotPhi * cos(thetaRad);
        float zRot = yRotPhi;  // Z после поворота вокруг Z

        return QVector3D( xRot, yRot, zRot );
    }

    QVector2D point2() {

        float angle = step * 2 * M_PI / TRAJECTORY_MAX_STEPS;

        // Координаты эллипса в локальной системе
        float x = (GLOBE_RADIUS + 0) * cos(angle);
        float y = (GLOBE_RADIUS + 0) * sin(angle);

        return QVector2D( x, y );
    }

    float alpha() {
        return step * 2 * M_PI / TRAJECTORY_MAX_STEPS;
    }

};

Q_DECLARE_METATYPE( Trajectory )

#endif //TRAJECTORY_H
