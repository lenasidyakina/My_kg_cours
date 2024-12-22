

#ifndef CURS_RENDERAREA_H
#define CURS_RENDERAREA_H


#include <QWidget>

#include "Cube.h"
#include "Figure.h"
#include "Trajectory.h"


class RenderArea : public QWidget {
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent);
    void invalidate();
protected:
    QBrush bkgndBrush;


    QPointF projectTo2D(const Point3D &point, float centerX, float centerY);


    void drawTrajectory(QPainter *painter, const Trajectory *trajectory);

    void paintEvent(QPaintEvent *event) override;


    const QList<Figure*> & FigureList();
};


#endif //CURS_RENDERAREA_H
