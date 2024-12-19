#include <QPainter>
#include <QVector3D>
#include <deque>
#include <cmath>
#include "RenderArea.h"
#include "SettingsDialog.h"
#include "MainWindow.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Tetrader.h"
#include "Trajectory.h"

Sphere * base_sphere = new Sphere(DEFAULT_FIGURE_COLOR, DEFAULT_BASE_SPHERE_RADIUS);

RenderArea::RenderArea(QWidget *parent)
        : QWidget(parent)
{
    bkgndBrush.setStyle(Qt::SolidPattern);
    bkgndBrush.setColor(SettingsDialog::backgroundColor());
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void RenderArea::invalidate()
{
    bkgndBrush.setColor(SettingsDialog::backgroundColor());
    update();
}

const QList<Figure*> & RenderArea::FigureList()
{
    return MainWindow::FigureList();
}

// Преобразование сферических координат в декартовые
Point3D sphericalToCartesian(float radius, float theta, float phi) {
    return Point3D(
        radius * sin(theta) * cos(phi),
        radius * sin(theta) * sin(phi),
        radius * cos(theta)
    );
}

QPointF RenderArea::projectTo2D(const Point3D& point, float centerX, float centerY) {
    float fov = 1000.0f; // Поле зрения
    float depth = point.z + fov; // Учитываем смещение по оси Z
    if (depth <= 0) depth = 0.1f; // Предотвращаем деление на ноль

    float projectedX = (point.x / depth) * fov + centerX;
    float projectedY = (point.y / depth) * fov + centerY;
    return QPointF(projectedX, projectedY);
}


QPointF projectTo2D(const Point3D& point, float centerX, float centerY) {
    float fov = 1000.0f; // Поле зрения
    float depth = point.z + fov; // Учитываем смещение по оси Z
    if (depth <= 0) depth = 0.1f; // Предотвращаем деление на ноль

    float projectedX = (point.x / depth) * fov + centerX;
    float projectedY = (point.y / depth) * fov + centerY;
    return QPointF(projectedX, projectedY);
}

Point3D crossProduct(const Point3D &a, const Point3D &b) {
    return Point3D(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

void normalize(Point3D &v) {
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    v.x /= length;
    v.y /= length;
    v.z /= length;
}


void Cube::Draw(QPainter *painter, Cube *cube, Sphere *sphere, Trajectory *trajectory) {
    painter->setPen(QPen(Qt::black)); // Цвет рёбер
    QColor cubeColor = QColor::fromRgb(cube->color);
    painter->setBrush(QBrush(cubeColor, Qt::SolidPattern));

    float phiRad = trajectory->phi * M_PI / 180.0f;
    float thetaRad = trajectory->theta * M_PI / 180.0f;
    int r = sphere->RadiusLength;

    float centerX = painter->device()->width() / 2.0f;
    float centerY = painter->device()->height() / 2.0f;

    QVector3D closestPoint3D;
    float closestZ = -std::numeric_limits<float>::max();
    const int steps = 360;
    for (int i = 0; i <= steps; i++) {
        float angle = i * 2 * M_PI / steps;
        float x = r * cos(angle);
        float y = r * sin(angle);
        float yRotPhi = y * cos(phiRad);
        float zRotPhi = y * sin(phiRad);
        float xRot = x * cos(thetaRad) - zRotPhi * sin(thetaRad);
        float yRot = x * sin(thetaRad) + zRotPhi * cos(thetaRad);
        float zRot = yRotPhi;

        if (zRot > closestZ) {
            closestZ = zRot;
            closestPoint3D = QVector3D(xRot, yRot, zRot);
        }
    }

    float size = cube->EdgeLength;

    QVector3D normalToPlane(sin(phiRad) * sin(thetaRad), -sin(phiRad) * cos(thetaRad), cos(phiRad));
    QVector3D axisParallel(cos(thetaRad), sin(thetaRad), 0);
    QVector3D axisPerpendicular = QVector3D::crossProduct(normalToPlane, axisParallel);

    QVector3D cubeBaseCenter = closestPoint3D - 2 * normalToPlane * (size / 2.0f);

    QVector3D vertices[8] = {
        cubeBaseCenter - axisParallel * size / 2 - axisPerpendicular * size / 2,
        cubeBaseCenter + axisParallel * size / 2 - axisPerpendicular * size / 2,
        cubeBaseCenter + axisParallel * size / 2 + axisPerpendicular * size / 2,
        cubeBaseCenter - axisParallel * size / 2 + axisPerpendicular * size / 2,
        cubeBaseCenter - axisParallel * size / 2 - axisPerpendicular * size / 2 + normalToPlane * size,
        cubeBaseCenter + axisParallel * size / 2 - axisPerpendicular * size / 2 + normalToPlane * size,
        cubeBaseCenter + axisParallel * size / 2 + axisPerpendicular * size / 2 + normalToPlane * size,
        cubeBaseCenter - axisParallel * size / 2 + axisPerpendicular * size / 2 + normalToPlane * size
    };

    QVector<QPointF> projectedVertices;
    for (const auto &v : vertices) {
        projectedVertices.append(QPointF(centerX + v.x(), centerY - v.y()));
    }

    int faces[6][4] = {
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {0, 1, 5, 4},
        {1, 2, 6, 5},
        {2, 3, 7, 6},
        {3, 0, 4, 7}
    };

    QVector3D faceNormals[6];
    for (int i = 0; i < 6; i++) {
        QVector3D v1 = vertices[faces[i][1]] - vertices[faces[i][0]];
        QVector3D v2 = vertices[faces[i][2]] - vertices[faces[i][0]];
        faceNormals[i] = QVector3D::crossProduct(v1, v2).normalized();
    }

    QVector3D observer(0, 0, -1); // Наблюдатель смотрит вдоль оси Z

    for (int i = 0; i < 6; i++) {
        if (QVector3D::dotProduct(faceNormals[i], observer) < 0) {
            QPolygonF polygon;
            for (int j = 0; j < 4; j++) {
                polygon.append(projectedVertices[faces[i][j]]);
            }
            painter->drawConvexPolygon(polygon); // Рисуем только видимые грани
        }
    }



    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    for (const auto &edge : edges) {
        bool visible = false;
        for (int i = 0; i < 6; i++) {
            if (QVector3D::dotProduct(faceNormals[i], observer) <= 0) {
                if (std::find(std::begin(faces[i]), std::end(faces[i]), edge[0]) != std::end(faces[i]) &&
                    std::find(std::begin(faces[i]), std::end(faces[i]), edge[1]) != std::end(faces[i])) {
                    visible = true;
                    break;
                }
            }
        }
        if (visible) {
            painter->drawLine(projectedVertices[edge[0]], projectedVertices[edge[1]]);
        }
    }
}

QPoint CalculateVertex(int x_center, int y_center, int radius, double theta, double phi) {
    int x = x_center + radius * sin(theta) * cos(phi);
    int y = y_center + radius * cos(theta);
    return QPoint(x, y);
}

// Функция для рисования рёбер
void drawEdge(QPainter *painter, QPoint p1, QPoint p2) {
    painter->drawLine(p1, p2);
}

void Sphere::Draw(QPainter *painter, Sphere *sphere) {
        painter->setRenderHint(QPainter::Antialiasing);

        // Устанавливаем цвет заливки
        painter->setBrush(QColor::fromRgb(sphere->color));
        painter->setPen(Qt::black);

        // Центрирование сферы в области рисования
        int x_center = painter->device()->width() / 2;
        int y_center = painter->device()->height() / 2;

        // Массив для вершин, представящих полигональные части сферы
        std::vector<QPoint> vertices;

        // Рисуем сферу как сетку полигонов (сектора по вертикали и горизонтали)
        for (int i = 0; i < sphere->stacks; ++i) {
            double theta1 = (i * M_PI) / sphere->stacks; // Угол по вертикали для первого ряда
            double theta2 = ((i + 1) * M_PI) / sphere->stacks; // Угол для второго ряда

            for (int j = 0; j < sphere->slices; ++j) {
                double phi1 = (j * 2 * M_PI) / sphere->slices; // Угол по горизонтали для первого сегмента
                double phi2 = ((j + 1) * 2 * M_PI) / sphere->slices; // Угол для второго сегмента

                // Вычисляем координаты вершин для текущих полигонов
                QPoint p1 = CalculateVertex(x_center, y_center, sphere->RadiusLength, theta1, phi1);
                QPoint p2 = CalculateVertex(x_center, y_center, sphere->RadiusLength, theta1, phi2);
                QPoint p3 = CalculateVertex(x_center, y_center, sphere->RadiusLength, theta2, phi2);
                QPoint p4 = CalculateVertex(x_center, y_center, sphere->RadiusLength, theta2, phi1);

                // Рисуем верхнюю половину квадрата
                QPolygon topPolygon;
                topPolygon << p1 << p2 << p3 << p4;
                painter->drawPolygon(topPolygon);
            }
        }

        // Рисуем рёбра (опционально)
        painter->setPen(Qt::gray);
        for (int i = 0; i < sphere->stacks; ++i) {
            double theta1 = (i * M_PI) / sphere->stacks;
            double theta2 = ((i + 1) * M_PI) / sphere->stacks;

            for (int j = 0; j < sphere->slices; ++j) {
                double phi1 = (j * 2 * M_PI) / sphere->slices;
                double phi2 = ((j + 1) * 2 * M_PI) / sphere->slices;

                QPoint p1 = CalculateVertex(x_center, y_center, sphere->RadiusLength, theta1, phi1);
                QPoint p2 = CalculateVertex(x_center, y_center, sphere->RadiusLength, theta1, phi2);
                QPoint p3 = CalculateVertex(x_center, y_center, sphere->RadiusLength, theta2, phi2);
                QPoint p4 = CalculateVertex(x_center, y_center, sphere->RadiusLength, theta2, phi1);

                // Прорисовываем рёбра
                drawEdge(painter, p1, p2);
                drawEdge(painter, p2, p3);
                drawEdge(painter, p3, p4);
                drawEdge(painter, p4, p1);
            }
        }
    }

void Cylinder::Draw(QPainter *painter, Cylinder *cylinder) {
    painter->setRenderHint(QPainter::Antialiasing);

    // Устанавливаем цвет заливки
    painter->setBrush(QColor::fromRgb(cylinder->color));
    painter->setPen(Qt::black);

    // Центрирование цилиндра в области рисования
    int height = cylinder->Height;
    int radius = cylinder->BaseRadius;
    int xCenter = painter->device()->width() / 2;
    int yCenter = painter->device()->height() / 2;

    // Смещение для эффекта трёхмерности
    int offset = height / 4;

    // Координаты для верхнего и нижнего кругов
    QPoint topCenter(xCenter, yCenter - height / 2);  // Верхний центр
    QPoint bottomCenter(xCenter, yCenter + height / 2);  // Нижний центр

    // Рисуем верхнюю и нижнюю грани цилиндра (круги)
    QPolygon topCircle, bottomCircle;

    // Количество секторов для аппроксимации кругов
    int sectors = 30;
    for (int i = 0; i < sectors; ++i) {
        double angle = (2 * M_PI * i) / sectors;
        int xTop = topCenter.x() + radius * cos(angle);
        int yTop = topCenter.y() + radius * sin(angle);
        int xBottom = bottomCenter.x() + radius * cos(angle);
        int yBottom = bottomCenter.y() + radius * sin(angle);

        topCircle << QPoint(xTop, yTop);
        bottomCircle << QPoint(xBottom, yBottom);
    }

    // Рисуем верхний и нижний круг
    painter->drawPolygon(topCircle);
    painter->drawPolygon(bottomCircle);

    // Рисуем боковые грани цилиндра (в виде прямоугольника)
    for (int i = 0; i < sectors; ++i) {
        // Индексы для текущей и следующей вершины
        int next = (i + 1) % sectors;

        // Рисуем боковой полигон
        QPolygon sideFace;
        sideFace << topCircle.at(i) << topCircle.at(next) << bottomCircle.at(next) << bottomCircle.at(i);
        painter->drawPolygon(sideFace);
    }

    // Прорисовываем рёбра (включая боковые рёбра цилиндра)
    painter->setPen(Qt::black);

    auto drawEdge = [&](QPoint p1, QPoint p2) {
        painter->drawLine(p1, p2);
    };

    // Рёбра верхнего и нижнего кругов
    for (int i = 0; i < sectors; ++i) {
        drawEdge(topCircle.at(i), topCircle.at((i + 1) % sectors));
        drawEdge(bottomCircle.at(i), bottomCircle.at((i + 1) % sectors));
    }

    // Рёбра боковой части
    for (int i = 0; i < sectors; ++i) {
        drawEdge(topCircle.at(i), bottomCircle.at(i));
    }
}


void Tetrader::Draw(QPainter *painter, Tetrader *tetrahedron) {
    painter->setRenderHint(QPainter::Antialiasing);

    // Устанавливаем цвет заливки
    painter->setBrush(QColor::fromRgb(tetrahedron->color));
    painter->setPen(Qt::black);

    // Высота тетраэдра
    int height = tetrahedron->Height;

    // Центрирование тетраэдра в области рисования
    int xCenter = painter->device()->width() / 2;
    int yCenter = painter->device()->height() / 2;

    // Вычисление размеров
    double edgeLength = (2.0 / sqrt(3)) * height; // Длина ребра тетраэдра
    int halfEdge = static_cast<int>(edgeLength / 2);

    // Координаты вершин (в проекции на 2D)
    QPoint top(xCenter, yCenter - height / 2);  // Верхняя вершина
    QPoint baseLeft(xCenter - halfEdge, yCenter + height / 2); // Левая вершина основания
    QPoint baseRight(xCenter + halfEdge, yCenter + height / 2); // Правая вершина основания
    QPoint baseBack(xCenter, yCenter + height / 4); // Третья вершина основания (сзади)

    // Рисуем грани тетраэдра

    // 1. Передняя грань
    QPolygon frontFace;
    frontFace << top << baseLeft << baseRight;
    painter->drawPolygon(frontFace);

    // 2. Левая грань
    QPolygon leftFace;
    leftFace << top << baseLeft << baseBack;
    painter->drawPolygon(leftFace);

    // 3. Правая грань
    QPolygon rightFace;
    rightFace << top << baseRight << baseBack;
    painter->drawPolygon(rightFace);

    // 4. Нижняя грань (основание)
    QPolygon bottomFace;
    bottomFace << baseLeft << baseRight << baseBack;
    painter->drawPolygon(bottomFace);

    // Прорисовываем рёбра
    painter->setPen(Qt::black);

    auto drawEdge = [&](QPoint p1, QPoint p2) {
        painter->drawLine(p1, p2);
    };

    // Рёбра тетраэдра
    drawEdge(top, baseLeft);    // Верхняя левая грань
    drawEdge(top, baseRight);   // Верхняя правая грань
    drawEdge(top, baseBack);    // Верхняя задняя грань
    drawEdge(baseLeft, baseRight); // Переднее ребро основания
    drawEdge(baseLeft, baseBack);  // Заднее левое ребро основания
    drawEdge(baseRight, baseBack); // Заднее правое ребро основания
}

void Sphere::Draw_base_sphere(QPainter *painter, Sphere *sphere) {
    painter->setRenderHint(QPainter::Antialiasing);

    // Устанавливаем цвет заливки
    painter->setBrush(QColor::fromRgb(sphere->color));
    painter->setPen(Qt::NoPen); // Убираем рёбра (границы)

    // Центрирование сферы в области рисования
    int x_center = painter->device()->width() / 2;
    int y_center = painter->device()->height() / 2;

    // Рисуем сферу как окружность
    int diameter = sphere->RadiusLength * 2;
    QRect sphereRect(x_center - sphere->RadiusLength,
                     y_center - sphere->RadiusLength,
                     diameter, diameter);
    painter->drawEllipse(sphereRect);
}

QPointF projectTo2D_t(float x, float y, float z, float centerX, float centerY) {
    // Проекция на 2D
    // Здесь предполагается, что z влияет на масштабирование (можно добавить перспективу)
    return QPointF(centerX + x, centerY - y);  // Преобразование для 2D
}

void RenderArea::drawTrajectory(QPainter *painter, const Trajectory *trajectory) {
    QColor color = QColor::fromRgb(trajectory->color);
    painter->setPen(QPen(color));

    float phiRad = trajectory->phi * M_PI / 180.0f;  // Угол наклона плоскости (phi)
    float thetaRad = trajectory->theta * M_PI / 180.0f;  // Угол отклонения (theta)
    int r = 100;  // Радиус сферы

    float centerX = painter->device()->width() / 2.0f;  // Центр экрана по X
    float centerY = painter->device()->height() / 2.0f;  // Центр экрана по Y

    QPointF lastPoint;
    bool lastVisible = false; // Видимость предыдущей точки
    const int steps = 360; // Количество точек для отрисовки эллипса

    for (int i = 0; i <= steps; i++) {
        float angle = i * 2 * M_PI / steps;  // Угол для генерации точек на эллипсе

        // Координаты эллипса в локальной системе
        float x = r * cos(angle);
        float y = r * sin(angle);

        // Поворот плоскости на угол phi вокруг оси X
        float yRotPhi = y * cos(phiRad);
        float zRotPhi = y * sin(phiRad);

        // Поворот на угол theta вокруг оси Z
        float xRot = x * cos(thetaRad) - zRotPhi * sin(thetaRad);
        float yRot = x * sin(thetaRad) + zRotPhi * cos(thetaRad);
        float zRot = yRotPhi;  // Z после поворота вокруг Z

        // Угол между точкой и наблюдателем
        float dotProduct = zRot; // Проверяем только Z-компонент, т.к. наблюдатель смотрит вдоль оси Z
        bool currentVisible = dotProduct >= 0; // Если точка "перед" наблюдателем

        // Преобразование в экранные координаты
        QPointF projectedPoint(centerX + xRot, centerY - yRot);

        // Если точка видима, рисуем линию
        if (i > 0 && (lastVisible || currentVisible)) {
            painter->drawLine(lastPoint, projectedPoint);
        }

        lastPoint = projectedPoint;
        lastVisible = currentVisible;
    }
}

void RenderArea::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setBrush(bkgndBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
    painter.setRenderHint(QPainter::Antialiasing);

    // Отрисовываем базовую сферу
    //Sphere * base_sphere = new Sphere(DEFAULT_FIGURE_COLOR, DEFAULT_BASE_SPHERE_RADIUS);
    //MainWindow::trajectory = new Trajectory(DEFAULT_TRAJECTORY_COLOR ,0,0);
    base_sphere->Draw(&painter, base_sphere);
    // Центр экрана
    float centerX = painter.device()->width() / 2.0f;
    float centerY = painter.device()->height() / 2.0f;

    // Радиус сферы
    float radius = base_sphere->RadiusLength;

    // Пример создания траектории
    //Trajectory trajectory("red", 30, 30);  // Пример с углами phi = 45, theta = 30, цвет - красный
    drawTrajectory(&painter, MainWindow::trajectory);

    // Рисуем траекторию
    //drawTrajectory(&painter, MainWindow::trajectory);

    // Отрисовываем фигуры
    for (Figure *fig : FigureList()) {
        int ft = fig->GetType();
        if (ft == FIGURE_CUBE) {
            Cube *cube = static_cast<Cube *>(fig);
            cube->Draw(&painter, cube, base_sphere, MainWindow::trajectory); // Используем функцию отрисовки куба
        } else if (ft == FIGURE_SPHERE) {
            Sphere *sphere = static_cast<Sphere *>(fig);
            sphere->Draw(&painter, sphere);
        } else if (ft == FIGURE_CYLINDER) {
            Cylinder *cylinder = static_cast<Cylinder *>(fig);
            cylinder->Draw(&painter, cylinder);
        } else if (ft == FIGURE_PYRAMID) {
            Tetrader *tetrader = static_cast<Tetrader *>(fig);
            tetrader->Draw(&painter, tetrader);
        }
    }
}



