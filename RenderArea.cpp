#include <QPainter>
#include <QVector3D>
#include <deque>
#include <cmath>
#include <QMatrix4x4>
#include <ctime>
#include "RenderArea.h"
#include "SettingsDialog.h"
#include "MainWindow.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Tetrader.h"
#include "Trajectory.h"

Sphere RenderArea::globe;

RenderArea::RenderArea(QWidget *parent)
        : QWidget(parent)
{
    bkgndBrush.setStyle(Qt::SolidPattern);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void RenderArea::invalidate()
{
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


void Cube::Draw(QPainter *painter, Cube *cube, Sphere *sphere,  std::vector<Poly> &polys) {
    painter->setPen(QPen(Qt::black));
    QColor cubeColor = QColor::fromRgb(cube->color);
    painter->setBrush(QBrush(cubeColor, Qt::SolidPattern));

    float centerX = painter->device()->width() / 2.0f + sphere->x;
    float centerY = painter->device()->height() / 2.0f + sphere->y;

    QVector3D closestPoint3D = MainWindow::trajectory->point(sphere->RadiusLength, cube->trainPosition);
    int r = sphere->RadiusLength;
    QVector3D normalizedPoint = closestPoint3D.normalized() * r;

    QVector3D normalToPlane = normalizedPoint.normalized();
    QVector3D tangent = MainWindow::trajectory->point(sphere->RadiusLength, cube->trainPosition + 1) - closestPoint3D;
    tangent.normalize();

    QVector3D perpendicular = QVector3D::crossProduct(normalToPlane, tangent).normalized();

    // Проверка стабильности ориентации
    if (perpendicular.z() < 0) {
        perpendicular = -perpendicular;
    }

    float size = cube->EdgeLength;

    // Центр нижней грани фиксируем на траектории
    QVector3D baseCenter = normalizedPoint;

    // Пересчёт вершин куба относительно фиксированной нижней грани
    QVector3D vertices[8] = {
            baseCenter - tangent * size / 2 - perpendicular * size / 2, // 0
            baseCenter + tangent * size / 2 - perpendicular * size / 2, // 1
            baseCenter + tangent * size / 2 + perpendicular * size / 2, // 2
            baseCenter - tangent * size / 2 + perpendicular * size / 2, // 3
            baseCenter - tangent * size / 2 - perpendicular * size / 2 + normalToPlane * size, // 4
            baseCenter + tangent * size / 2 - perpendicular * size / 2 + normalToPlane * size, // 5
            baseCenter + tangent * size / 2 + perpendicular * size / 2 + normalToPlane * size, // 6
            baseCenter - tangent * size / 2 + perpendicular * size / 2 + normalToPlane * size  // 7
    };

    // Расчёт координат центра куба (среднее значение всех вершин)
    QVector3D cubeCenter(0, 0, 0);
    for (int i = 0; i < 8; i++) {
        cubeCenter += vertices[i];
    }
    cubeCenter /= 8.0f;  // Среднее значение
    cube->center = cubeCenter;

    QVector<QVector3D> projectedVertices;
    for (const auto &v : vertices) {
        projectedVertices.append(QVector3D(centerX + v.x(), centerY - v.y(), v.z()));
    }

    int faces[6][4] = {
            {0, 1, 2, 3}, {4, 5, 6, 7}, {0, 1, 5, 4},
            {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 0, 4, 7}
    };

    QVector3D faceNormals[6];
    for (int i = 0; i < 6; i++) {
        QVector3D v1 = vertices[faces[i][1]] - vertices[faces[i][0]];
        QVector3D v2 = vertices[faces[i][2]] - vertices[faces[i][0]];
        faceNormals[i] = QVector3D::crossProduct(v1, v2).normalized();
    }

    // Отрисовка граней
    for (int i = 0; i < 6; i++) {
        bool isBottomFace = false;

        Poly polygon;
        for (int j = 0; j < 4; j++) {
            polygon.push_back(projectedVertices[faces[i][j]]);
        }
        polygon.set_color(color);
        std::reverse(std::begin(polygon), std::end(polygon));
        polys.push_back(polygon);
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

void Sphere::Draw(QPainter *painter, Sphere *globe, Sphere *sphere, std::vector<Poly> &polys) {
    painter->setPen(QPen(Qt::black));

    // Центр устройства (экран отрисовки)
    float centerX = painter->device()->width() / 2.0f + globe->x;
    float centerY = painter->device()->height() / 2.0f + globe->y;

    // Радиус базовой сферы
    float baseRadius = globe->RadiusLength;

    // Получаем точку на базовой сфере по траектории
    QVector3D trajectoryPoint = MainWindow::trajectory->point(baseRadius, trainPosition); // getTrajectoryPoint(trajectory, trajectory->step, 360);

    // Нормализуем точку и масштабируем её радиусом базовой сферы
    QVector3D baseSpherePoint = trajectoryPoint.normalized() * baseRadius;

    // Радиус движущейся сферы
    float movingSphereRadius = sphere->RadiusLength;

    // Сдвигаем центр движущейся сферы вверх на радиус движущейся сферы
    QVector3D movingSphereCenter = baseSpherePoint + trajectoryPoint.normalized() * movingSphereRadius;

    // Вычисляем матрицу вращения
    QMatrix4x4 rotationMatrix;
    QVector3D upVector(0, 0, 1); // Исходная ось вверх
    QVector3D normal = trajectoryPoint.normalized();

    if (!qFuzzyCompare(normal, upVector)) {
        QVector3D rotationAxis = QVector3D::crossProduct(upVector, normal).normalized();
        float angle = acos(QVector3D::dotProduct(upVector, normal));
        rotationMatrix.rotate(qRadiansToDegrees(angle), rotationAxis);
    }

    // Разбиение движущейся сферы на сегменты
    int latitudeSegments = 20;   // Сегменты по широте
    int longitudeSegments = 20; // Сегменты по долготе

    QVector<QVector3D> vertices;
    for (int i = 0; i <= latitudeSegments; ++i) {
        float theta = M_PI * i / latitudeSegments; // Угол широты
        for (int j = 0; j <= longitudeSegments; ++j) {
            float phi = 2.0f * M_PI * j / longitudeSegments; // Угол долготы

            // Вычисление позиции вершины движущейся сферы
            float x = movingSphereRadius * sin(theta) * cos(phi);
            float y = movingSphereRadius * sin(theta) * sin(phi);
            float z = movingSphereRadius * cos(theta);

            QVector3D localVertex(x, y, z);
            QVector3D rotatedVertex = rotationMatrix * localVertex + movingSphereCenter;

            vertices.append(rotatedVertex);
        }
    }

    // Преобразование вершин в 2D-координаты для отрисовки
    QVector<QVector3D> projectedVertices;
    for (const auto &vertex : vertices) {
        projectedVertices.append(QVector3D(centerX + vertex.x(), centerY - vertex.y(), vertex.z()));
    }

    // Отрисовка полигонов (граней между сегментами)
    for (int i = 0; i < latitudeSegments; ++i) {
        for (int j = 0; j < longitudeSegments; ++j) {
            int index1 = i * (longitudeSegments + 1) + j;
            int index2 = index1 + longitudeSegments + 1;
            Poly p = { projectedVertices[index1], projectedVertices[index1 + 1], projectedVertices[index2 + 1], projectedVertices[index2] };
            p.set_color(color);
            polys.push_back(p);
            /*QPolygonF polygon;
            polygon.append(projectedVertices[index1]);
            polygon.append(projectedVertices[index1 + 1]);
            polygon.append(projectedVertices[index2 + 1]);
            polygon.append(projectedVertices[index2]);

            painter->drawConvexPolygon(polygon);*/
        }
    }
}


void Cylinder::Draw(QPainter *painter, Sphere *globe, Cylinder *cylinder, std::vector<Poly> &polys) {
    painter->setRenderHint(QPainter::Antialiasing);

    // Устанавливаем цвет заливки
    painter->setBrush(QColor::fromRgb(cylinder->color));
    painter->setPen(Qt::black);

    // Центрирование области рисования
    float centerX = painter->device()->width() / 2.0f + globe->x;
    float centerY = painter->device()->height() / 2.0f + globe->y;

    // Радиус базовой сферы
    float baseRadius = globe->RadiusLength;

    // Получаем точку на траектории
    QVector3D trajectoryPoint = MainWindow::trajectory->point(baseRadius, trainPosition);

    // Нормализуем точку и масштабируем до радиуса базовой сферы
    QVector3D baseSpherePoint = trajectoryPoint.normalized() * baseRadius;

    // Радиус и высота цилиндра
    float cylinderRadius = cylinder->BaseRadius;
    float cylinderHeight = cylinder->Height;

    // Нижняя окружность цилиндра касается сферы
    QVector3D cylinderBottomCenter = baseSpherePoint + trajectoryPoint.normalized();

    // Верхняя окружность цилиндра
    QVector3D cylinderTopCenter = cylinderBottomCenter + trajectoryPoint.normalized() * cylinderHeight;

    // Вычисляем матрицу вращения цилиндра
    QMatrix4x4 rotationMatrix;
    QVector3D upVector(0, 0, 1); // Исходная ось вверх
    QVector3D normal = trajectoryPoint.normalized();

    if (!qFuzzyCompare(normal, upVector)) {
        QVector3D rotationAxis = QVector3D::crossProduct(upVector, normal).normalized();
        float angle = acos(QVector3D::dotProduct(upVector, normal));
        rotationMatrix.rotate(qRadiansToDegrees(angle), rotationAxis);
    }

    // Количество секторов для аппроксимации круга
    int sectors = 30;
    QVector<QVector3D> bottomVertices, topVertices;

    for (int i = 0; i < sectors; ++i) {
        double angle = (2.0 * M_PI * i) / sectors;

        // Вычисляем точки нижней и верхней окружностей
        QVector3D localBottom(cylinderRadius * cos(angle), cylinderRadius * sin(angle), 0);
        QVector3D localTop(cylinderRadius * cos(angle), cylinderRadius * sin(angle), cylinderHeight);

        bottomVertices.append(rotationMatrix * localBottom + cylinderBottomCenter);
        topVertices.append(rotationMatrix * localTop + cylinderBottomCenter);
    }

    // Преобразование в 2D для отрисовки
    Poly projectedBottom, projectedTop;
    for (const auto &vertex : bottomVertices) {
        projectedBottom.push_back(QVector3D(centerX + vertex.x(), centerY - vertex.y(), vertex.z()));
    }
    for (const auto &vertex : topVertices) {
        projectedTop.push_back(QVector3D(centerX + vertex.x(), centerY - vertex.y(), vertex.z()));
    }

    // Отрисовка верхнего и нижнего кругов
    //painter->drawPolygon(projectedBottom);
    //painter->drawPolygon(projectedTop);
    // Рисуем боковые грани цилиндра
    for (int i = 0; i < sectors; ++i) {
        int next = (i + 1) % sectors;
        Poly poly = { projectedBottom.at(i), projectedBottom.at(next), projectedTop.at(next), projectedTop.at(i) };
        std::reverse(std::begin(poly), std::end(poly));
        poly.set_color(color);
        polys.push_back(poly);
        /*QPolygonF sideFace;
        sideFace << projectedBottom.at(i) << projectedBottom.at(next)
                 << projectedTop.at(next) << projectedTop.at(i);
        painter->drawPolygon(sideFace);*/
    }

    // Прорисовка рёбер цилиндра
    painter->setPen(Qt::black);

    auto drawEdge = [&](QPointF p1, QPointF p2) {
        painter->drawLine(p1, p2);
    };

    /*for (int i = 0; i < sectors; ++i) {
        drawEdge(projectedBottom.at(i), projectedBottom.at((i + 1) % sectors));
        drawEdge(projectedTop.at(i), projectedTop.at((i + 1) % sectors));
        drawEdge(projectedBottom.at(i), projectedTop.at(i));
    }*/
    std::reverse(std::begin(projectedTop), std::end(projectedTop));
    std::reverse(std::begin(projectedBottom), std::end(projectedBottom));

    projectedTop.set_color(color);
    polys.push_back(projectedTop);

    projectedBottom.set_color(color);
    polys.push_back(projectedBottom);
}

#if 0
void Tetrader::Draw(QPainter *painter, Sphere *globe, Tetrader *tetrahedron, std::vector<Poly> &polys) {
    painter->setRenderHint(QPainter::Antialiasing);

    // Устанавливаем цвет заливки
    painter->setBrush(QColor::fromRgb(tetrahedron->color));
    painter->setPen(Qt::black);

    // Центрирование области рисования
    float centerX = painter->device()->width() / 2.0f + globe->x;
    float centerY = painter->device()->height() / 2.0f + globe->y;

    // Радиус базовой сферы
    float baseRadius = globe->RadiusLength;

    // Получаем точку на траектории
    QVector3D trajectoryPoint = MainWindow::trajectory->point(baseRadius, trainPosition); //getTrajectoryPoint(trajectory, trajectory->step, 360);

    // Нормализуем точку и масштабируем до радиуса базовой сферы
    QVector3D baseSpherePoint = trajectoryPoint.normalized() * baseRadius;

    // Высота тетраэдра
    float height = tetrahedron->Height;

    // Длина ребра правильного тетраэдра
    float edgeLength = (2.0 / sqrt(3)) * height;

    // Определяем центр нижней грани тетраэдра
    QVector3D baseCenter = baseSpherePoint;

    // Вычисляем смещение от центра нижней грани к вершине
    QVector3D apexOffset = trajectoryPoint.normalized() * height;

    // Верхняя вершина тетраэдра
    QVector3D apex = baseCenter + apexOffset;

    // Нормаль к траектории (вектор перпендикулярный траектории)
    QVector3D normal = QVector3D::crossProduct(trajectoryPoint, QVector3D(0, 0, 1)).normalized();

    // Используем нормаль для правильной ориентации рёбер
    QVector3D tangent1 = normal;
    QVector3D tangent2 = QVector3D::crossProduct(tangent1, trajectoryPoint).normalized();

    // Вершины нижней грани
    QVector3D baseVertex1 = baseCenter + tangent1 * (edgeLength / 2);
    QVector3D baseVertex2 = baseCenter - tangent1 * (edgeLength / 2) + tangent2 * (edgeLength / 2);
    QVector3D baseVertex3 = baseCenter - tangent1 * (edgeLength / 2) - tangent2 * (edgeLength / 2);

    // Преобразование в 2D для отрисовки
    auto projectTo2D = [&](const QVector3D &point) -> QPointF {
        return QPointF(centerX + point.x(), centerY - point.y());
    };

    QPointF projectedApex = projectTo2D(apex);
    QPointF projectedBase1 = projectTo2D(baseVertex1);
    QPointF projectedBase2 = projectTo2D(baseVertex2);
    QPointF projectedBase3 = projectTo2D(baseVertex3);

    // Рисуем грани тетраэдра

    // 1. Передняя грань
    QPolygonF frontFace;
    frontFace << projectedApex << projectedBase1 << projectedBase2;
    // painter->drawPolygon(frontFace);
    polys.push_back({ apex, baseVertex1, baseVertex2 });

    // 2. Левая грань
    QPolygonF leftFace;
    leftFace << projectedApex << projectedBase1 << projectedBase3;
    // painter->drawPolygon(leftFace);
    polys.push_back({ apex, baseVertex1, baseVertex3 });

    // 3. Правая грань
    QPolygonF rightFace;
    rightFace << projectedApex << projectedBase2 << projectedBase3;
    // painter->drawPolygon(rightFace);
    polys.push_back({ apex, baseVertex2, baseVertex3 });

    // 4. Нижняя грань
    QPolygonF bottomFace;
    bottomFace << projectedBase1 << projectedBase2 << projectedBase3;
    // painter->drawPolygon(bottomFace);
    polys.push_back({ baseVertex1, baseVertex2, baseVertex3 });

    // Прорисовываем рёбра
    //painter->setPen(Qt::black);

    auto drawEdge = [&](QPointF p1, QPointF p2) {
        painter->drawLine(p1, p2);
    };

    // Рёбра тетраэдра
    //drawEdge(projectedApex, projectedBase1);
    //drawEdge(projectedApex, projectedBase2);
    //drawEdge(projectedApex, projectedBase3);
    //drawEdge(projectedBase1, projectedBase2);
    //drawEdge(projectedBase1, projectedBase3);
    //drawEdge(projectedBase2, projectedBase3);
}

#endif

static QVector3D _kostil(QVector3D&& vector)
{
    vector.setY(-vector.y());
    return vector;
}

void Tetrader::Draw(QPainter *painter, Sphere *globe, Tetrader *tetrahedron,  std::vector<Poly> &polys) {
    painter->setRenderHint(QPainter::Antialiasing);
    // Устанавливаем цвет заливки
    painter->setBrush(QColor::fromRgb(tetrahedron->color));
    painter->setPen(Qt::black);

    // Центрирование области рисования
    float centerX = painter->device()->width() / 2.0f + globe->x;
    float centerY = painter->device()->height() / 2.0f + globe->y;
    QVector3D center = {centerX, centerY, 0};


    // Радиус базовой сферы
    float baseRadius = globe->RadiusLength;

    // Получаем точку на траектории
    QVector3D trajectoryPoint = MainWindow::trajectory->point(baseRadius, trainPosition);

    // Нормализуем точку и масштабируем до радиуса базовой сферы
    QVector3D baseSpherePoint = trajectoryPoint.normalized() * baseRadius;

    // Высота тетраэдра
    float height = tetrahedron->Height;

    // Длина ребра правильного тетраэдра
    float edgeLength = (2.0 / sqrt(3)) * height;

    // Определяем центр нижней грани тетраэдра
    QVector3D baseCenter = baseSpherePoint;

    // Вычисляем смещение от центра нижней грани к вершине
    QVector3D apexOffset = trajectoryPoint.normalized() * height;

    // Верхняя вершина тетраэдра
    QVector3D apex = center + _kostil(baseSpherePoint + apexOffset);

    // Нормаль к траектории (вектор перпендикулярный траектории)
    QVector3D normal = QVector3D::crossProduct(trajectoryPoint, QVector3D(0, 0, 1)).normalized();


    // Используем нормаль для правильной ориентации рёбер
    QVector3D tangent1 = normal;
    QVector3D tangent2 = QVector3D::crossProduct(tangent1, trajectoryPoint).normalized();

    // Вершины нижней грани
    QVector3D baseVertex1 = center + _kostil(baseCenter + tangent1 * (edgeLength / 2));
    QVector3D baseVertex2 = center + _kostil(baseCenter - tangent1 * (edgeLength / 2) + tangent2 * (edgeLength / 2));
    QVector3D baseVertex3 = center + _kostil(baseCenter - tangent1 * (edgeLength / 2) - tangent2 * (edgeLength / 2));

    qDebug() << apex << baseVertex1 << baseVertex2 << baseVertex3;
    // Рисуем грани тетраэдра

    // 1. Передняя грань
    QPolygonF frontFace;
    //frontFace << projectedApex << projectedBase1 << projectedBase2;
    // painter->drawPolygon(frontFace);
    Poly p = { apex, baseVertex1, baseVertex2 };
    p.set_color(color);
    polys.push_back(p);

    // 2. Левая грань
    QPolygonF leftFace;
    //leftFace << projectedApex << projectedBase1 << projectedBase3;
    // painter->drawPolygon(leftFace);
    p = { apex, baseVertex1, baseVertex3 };
    p.set_color(color);
    polys.push_back(p);

    // 3. Правая грань
    QPolygonF rightFace;
    //rightFace << projectedApex << projectedBase2 << projectedBase3;
    // painter->drawPolygon(rightFace);
    p = { apex, baseVertex2, baseVertex3 };
    p.set_color(color);
    polys.push_back(p);

    // 4. Нижняя грань
    QPolygonF bottomFace;
    //bottomFace << projectedBase1 << projectedBase2 << projectedBase3;
    // painter->drawPolygon(bottomFace);
    p = { baseVertex1, baseVertex2, baseVertex3 };
    p.set_color(color);
    polys.push_back(p);
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
    int r = globe.RadiusLength;

    float centerX = painter->device()->width() / 2.0f + globe.x;  // Центр экрана по X
    float centerY = painter->device()->height() / 2.0f + globe.y;  // Центр экрана по Y

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
        if(i == 0)
            painter->drawPoint(projectedPoint);

        // Если точка видима, рисуем линию
        if ((i > 0 && (lastVisible || currentVisible)) || (i > 0 && (abs(trajectory->theta) < EPS)) && ((abs(trajectory->phi) - 90) < EPS)) {
            painter->drawLine(lastPoint, projectedPoint);
        }

        lastPoint = projectedPoint;
        lastVisible = currentVisible;
    }
}

QVector3D RenderArea::getTrajectoryPoint(const Trajectory *trajectory) {

    float phiRad = trajectory->phi * M_PI / 180.0f;  // Угол наклона плоскости (phi)
    float thetaRad = trajectory->theta * M_PI / 180.0f;  // Угол отклонения (theta)

    int step = trajectory->step;

    float angle = step * 2 * M_PI / TRAJECTORY_MAX_STEPS;

    // Координаты эллипса в локальной системе
    float x = GLOBE_RADIUS * cos(angle);
    float y = GLOBE_RADIUS * sin(angle);

    // Поворот плоскости на угол phi вокруг оси X
    float yRotPhi = y * cos(phiRad);
    float zRotPhi = y * sin(phiRad);

    // Поворот на угол theta вокруг оси Z
    float xRot = x * cos(thetaRad) - zRotPhi * sin(thetaRad);
    float yRot = x * sin(thetaRad) + zRotPhi * cos(thetaRad);
    float zRot = yRotPhi;  // Z после поворота вокруг Z

    return QVector3D( xRot, yRot, zRot );
}



void swap(int* xp, int* yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int geta(int step) {
    int a;
    if (step > 270) {
        a = step - 270;
    }
    else if (step > 180) {
        a = 270 - step;
    }
    else if (step < 90) {
        a = step + 90;
    }
    else
        a = 270 - step;
    return a;
}

void sortFigures(Figure * arr[], int n){
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            int a = geta(arr[j]->trainPosition);
            int b = geta(arr[j+1]->trainPosition);


            if (a > b) {
                Figure * tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                swapped = true;
            }
        }

        // If no two elements were swapped by inner loop,
        // then break
        if (swapped == false)
            break;
    }
}


void Sphere::DrawGlobe(QPainter *painter, Sphere *sphere, std::vector<Poly> &polys) {
    painter->setRenderHint(QPainter::Antialiasing);

    float x_center = painter->device()->width() / 2 + sphere->x;
    float y_center = painter->device()->height() / 2 + sphere->y;
    float z_center = 0;

    std::vector<QVector3D> vertices;
    vertices.push_back(QVector3D(x_center, y_center + sphere->RadiusLength, 0.0f));

    // Создание вершин для "поясов"
    for (int i = 0; i < sphere->stacks - 1; ++i) {
        double phi = M_PI * double(i + 1) / double(sphere->stacks);
        for (int j = 0; j < sphere->slices; ++j) {
            double theta = 2.0 * M_PI * double(j) / double(sphere->slices);
            float x = x_center + sphere->RadiusLength * std::sin(phi) * std::cos(theta);
            float y = y_center + sphere->RadiusLength * std::cos(phi);
            float z = z_center + sphere->RadiusLength * std::sin(phi) * std::sin(theta);
            vertices.emplace_back(x, y, z);
        }
    }
    vertices.push_back(QVector3D(x_center, y_center - sphere->RadiusLength, 0.0f));

    for (int i = 0; i < sphere->slices; ++i) {
        int i0 = i + 1;
        int i1 = (i + 1) % sphere->slices + 1;

        Poly poly_top = { vertices[0], vertices[i1], vertices[i0] };
        polys.push_back(poly_top);

        i0 = i + sphere->slices * (sphere->stacks - 2) + 1;
        i1 = (i + 1) % sphere->slices + sphere->slices * (sphere->stacks - 2) + 1;

        Poly poly_bottom = { vertices[vertices.size() - 1], vertices[i0], vertices[i1] };
        polys.push_back(poly_bottom);
    }

    // Вершины для остальных частей сферы
    for (int j = 0; j < sphere->stacks - 2; ++j) {
        int j0 = j * sphere->slices + 1;
        int j1 = (j + 1) * sphere->slices + 1;
        for (int i = 0; i < sphere->slices; ++i) {
            int i0 = j0 + i;
            int i1 = j0 + (i + 1) % sphere->slices;
            int i2 = j1 + (i + 1) % sphere->slices;
            int i3 = j1 + i;

            Poly poly = { vertices[i0], vertices[i1], vertices[i2], vertices[i3] };
            poly.set_color(color);
            polys.push_back(poly);
        }
    }
}


void RenderArea::computeTrain()
{
    int pos = MainWindow::trajectory->step;
    int dlen = 0;
    int tlen = 0;
    int radius = globe.RadiusLength;
    for (Figure *fig : FigureList()) {
        if (fig->GetType() == FIGURE_CUBE) {
            dlen = (180.0*(static_cast<Cube *>(fig)->EdgeLength + 5)/2)/(M_PI*radius);
        }
        else if (fig->GetType() == FIGURE_SPHERE) {
            dlen = (180.0*static_cast<Sphere *>(fig)->RadiusLength)/(M_PI*radius);
        }
        else if (fig->GetType() == FIGURE_PYRAMID) {
            dlen = (180.0*static_cast<Tetrader *>(fig)->Height/1.5)/(M_PI*radius);
        }
        else if (fig->GetType() == FIGURE_CYLINDER) {
            dlen = (180.0*static_cast<Cylinder *>(fig)->BaseRadius)/(M_PI*radius);
        }
        pos =  (TRAJECTORY_MAX_STEPS + pos - dlen - tlen) % TRAJECTORY_MAX_STEPS;
        fig->trainPosition = pos;
        tlen = dlen;
    }
}

QPointF RenderArea::getCenter(QPainter * painter)
{
    return QPointF(painter->device()->width() / 2.0f + globe.x,
                   painter->device()->height() / 2.0f + globe.y);
}

void RenderArea::paintEvent(QPaintEvent *) {
    clock_t start_time = clock();  // Сохраняем время начала
    std::vector<Poly> m_polys;

    QPainter painter(this);
    painter.setBrush(bkgndBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
    painter.setRenderHint(QPainter::Antialiasing);

    // Отрисовываем базовую сферу
    //Sphere * base_sа яphere = new Sphere(DEFAULT_FIGURE_COLOR, DEFAULT_BASE_SPHERE_RADIUS);
    //MainWindow::trajectory = new Trajectory(DEFAULT_TRAJECTORY_COLOR ,0,0);
    globe.DrawGlobe(&painter, &globe, m_polys);

    computeTrain();
    float centerX = painter.device()->width() / 2.0f + globe.x;
    float centerY = painter.device()->height() / 2.0f + globe.y;

    // Отрисовываем фигуры
    // z_buffer->fill(std::numeric_limits<float>::min());
    for (Figure *fig : FigureList()) {
        int ft = fig->GetType();
        if (ft == FIGURE_CUBE) {
            Cube *cube = static_cast<Cube *>(fig);
            cube->Draw(&painter, cube, &globe,  m_polys); // Используем функцию отрисовки куба
        }
        else if (ft == FIGURE_SPHERE) {
            Sphere *sphere = static_cast<Sphere *>(fig);
            sphere->Draw(&painter, &globe, sphere,  m_polys);
        }
        else if (ft == FIGURE_CYLINDER) {
            Cylinder *cylinder = static_cast<Cylinder *>(fig);
            cylinder->Draw(&painter, &globe, cylinder, m_polys);
        }
        else if (ft == FIGURE_PYRAMID) {
            Tetrader *tetrader = static_cast<Tetrader *>(fig);
            tetrader->Draw(&painter, &globe, tetrader,  m_polys);
        }
    }

    std::shared_ptr<QGenericMatrix<2000, 2000, float>> z_buffer = std::make_shared<QGenericMatrix<2000, 2000, float>>();
    z_buffer->fill(std::numeric_limits<float>::lowest());
    const QVector3D light(centerX + 100, centerY - 150, 150);
    for (const auto &poly : m_polys)
    {
        const auto norm = -poly.normal().normalized();
        poly.for_each_pixel([&](int x, int y, float z) {
            if ((x < 0) || (y < 0))
                return;
            if ((*z_buffer)(x, y) > z)
                return;
            (*z_buffer)(x, y) = z;

            painter.setPen(QPen(QColor(0, 255, 0)));
            const QVector3D lightDir = (QVector3D(x, y, z) - light).normalized();
            float intensity = std::clamp(QVector3D::dotProduct(norm, lightDir), 0.1f, 1.f);
            /*for (const auto &_poly : m_polys)
            {
                if (_poly == poly)
                    continue;
                const auto intersection = _poly.intersect(light, lightDir);
                if (intersection.has_value())
                {
                    if (std::abs(light.z() - intersection.value().z()) < std::abs(light.z() - z))
                    {
                        intensity = 0.1f;
                        break;
                    }
                }
            }*/

            QColor color = poly.get_color();
            painter.setPen(QPen(QColor(color.red() * intensity, color.green() * intensity, color.blue() * intensity)));
            painter.drawPoint(QPointF(x, y));
        });
    }
    painter.setPen(QPen(QColor(255, 255, 0)));
    painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern)); // Задаём цвет движущейся сферы
    painter.drawEllipse(light.x(), light.y(), 20, 20);
    // Пример создания траектории
    //Trajectory trajectory("red", 30, 30);  // Пример с углами phi = 45, theta = 30, цвет - красный
    if (MainWindow::trajectory->flag)
        drawTrajectory(&painter, MainWindow::trajectory);
    clock_t end_time = clock();  // Сохраняем время окончания
    double elapsed_time = double(end_time - start_time) / CLOCKS_PER_SEC;  // Вычисляем прошедшее время в секундах

    //qDebug() << "Time taken for paintEvent:" << elapsed_time << "seconds";  // Выводим время в секундах
}
