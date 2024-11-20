//
// Created by Ivan Sidyakin on 10.11.2024.
//

#include <QPainter>
#include <cmath>
#include "RenderArea.h"
#include "SettingsDialog.h"
#include "MainWindow.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Tetrader.h"


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

// Куб
void Cube::Draw(QPainter* painter, Cube* cube, Sphere* sphere) {
    painter->setRenderHint(QPainter::Antialiasing);

    // Цвет куба
    painter->setBrush(QColor::fromRgb(cube->color));
    painter->setPen(Qt::black);

    // Центр экрана
    float centerX = painter->device()->width() / 2.0f;
    float centerY = painter->device()->height() / 2.0f;

    // Позиция нижней грани куба относительно сферы
    Point3D bottomCenter = sphericalToCartesian(sphere->RadiusLength, cube->theta, cube->phi);

    // Размер куба
    float edge = cube->EdgeLength;
    float halfEdge = edge / 2.0f;

    // Вычисление вершин куба относительно его центра
    Point3D vertices[8] = {
        {bottomCenter.x - halfEdge, bottomCenter.y - halfEdge, bottomCenter.z - halfEdge},
        {bottomCenter.x + halfEdge, bottomCenter.y - halfEdge, bottomCenter.z - halfEdge},
        {bottomCenter.x + halfEdge, bottomCenter.y + halfEdge, bottomCenter.z - halfEdge},
        {bottomCenter.x - halfEdge, bottomCenter.y + halfEdge, bottomCenter.z - halfEdge},
        {bottomCenter.x - halfEdge, bottomCenter.y - halfEdge, bottomCenter.z + halfEdge},
        {bottomCenter.x + halfEdge, bottomCenter.y - halfEdge, bottomCenter.z + halfEdge},
        {bottomCenter.x + halfEdge, bottomCenter.y + halfEdge, bottomCenter.z + halfEdge},
        {bottomCenter.x - halfEdge, bottomCenter.y + halfEdge, bottomCenter.z + halfEdge},
    };

    // Функция проекции 3D в 2D
    auto projectTo2D = [&](const Point3D& point) {
        float fov = 1000.0f; // Поле зрения
        float depth = point.z + fov; // Учитываем смещение по оси Z
        if (depth <= 0) depth = 0.1f; // Предотвращаем деление на ноль

        float projectedX = (point.x / depth) * fov + centerX;
        float projectedY = (point.y / depth) * fov + centerY;
        return QPointF(projectedX, projectedY);
    };

    // Рёбра куба
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Нижняя грань
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Верхняя грань
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Соединительные рёбра
    };

    // Рисуем рёбра куба
    for (const auto& edge : edges) {
        QPointF p1 = projectTo2D(vertices[edge[0]]);
        QPointF p2 = projectTo2D(vertices[edge[1]]);
        painter->drawLine(p1, p2);
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
        painter->setPen(Qt::black);
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


void RenderArea::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setBrush(bkgndBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    Sphere * base_sphere = new Sphere(DEFAULT_FIGURE_COLOR, DEFAULT_BASE_SPHERE_RADIUS);
    base_sphere->Draw_base_sphere(&painter, base_sphere);

    for (Figure *fig : FigureList()) {
        int ft = fig->GetType();
        if (ft == FIGURE_CUBE) {
            Cube *cube = static_cast<Cube *>(fig);
            cube->Draw(&painter, cube, base_sphere); // Используем функцию отрисовки куба
        } else if (ft == FIGURE_SPHERE) {
            Sphere *sphere = static_cast<Sphere *>(fig);
            sphere->Draw(&painter, sphere);
        } else if (ft == FIGURE_CYLINDER) {
            Cylinder *cylinder = static_cast<Cylinder *>(fig);
            cylinder->Draw(&painter, cylinder);
        }
        else if (ft == FIGURE_PYRAMID) {
            Tetrader *tetrader = static_cast<Tetrader *>(fig);
            tetrader->Draw(&painter, tetrader);
        }
    }
}



