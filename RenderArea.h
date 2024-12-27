//
// Created by Ivan Sidyakin on 10.11.2024.
//

#ifndef CURS_RENDERAREA_H
#define CURS_RENDERAREA_H


#include <QWidget>
#include <QSettings>

#include "Cube.h"
#include "Figure.h"
#include "Trajectory.h"
#include "SettingsDialog.h"

#define GLOBE_RADIUS 100
#define DEFAULT_GLOBE_RADIUS 100
#define DEFAULT_GLOBE_COLOR "#44FF44"

class RenderArea : public QWidget {
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent);
    void invalidate();
    int getTrainSpeed() {
        if (trainSpeed < 1)
            trainSpeed = 1;
        else if (trainSpeed > 90)
            trainSpeed = 90;
        return trainSpeed;
    }
    void updateSettings()
    {
        QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
        globe.x = settings.value("globe-center-x", 0).toInt();
        globe.y = settings.value("globe-center-y", 0).toInt();
        globe.RadiusLength = settings.value("globe-radius", DEFAULT_GLOBE_RADIUS).toUInt();
        globe.color = Figure::ColorToInt( settings.value("globe-color", DEFAULT_GLOBE_COLOR).toString());
        trainSpeed = settings.value("train-speed", 1).toInt();
        QString backGroundColor = settings.value("background-color", "#000000").toString();
        bkgndBrush.setColor(backGroundColor);
        this->repaint();
    }
    static Sphere globe;

protected:
    QBrush bkgndBrush;
    int trainSpeed = 1;

    QPointF projectTo2D(const Point3D &point, float centerX, float centerY);
    void computeTrain();
    QPointF getCenter(QPainter * painter);

    void drawTrajectory(QPainter *painter, const Trajectory *trajectory);

    void paintEvent(QPaintEvent *event) override;

    QVector3D getTrajectoryPoint(const Trajectory *trajectory);

    const QList<Figure*> & FigureList();
};


#endif //CURS_RENDERAREA_H
