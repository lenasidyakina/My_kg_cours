
#include "Cube.h"


#include <QPainter>

void Cube::Draw()
{

}


QString Cube::getName()
{
    return "Куб " + QString("%1px").arg(this->EdgeLength);
}

void Cube::updateVertices(const Point3D& center, const Point3D& tangent, const Point3D& up, const Point3D& normal) {
    float halfEdge = EdgeLength / 2.0f;

    // Базовые векторы для ориентации куба
    Point3D right = tangent * halfEdge;
    Point3D upVec = up * halfEdge;
    Point3D forward = normal * halfEdge;

    // Обновляем позиции вершин куба
    vertices.clear();
    vertices = {
        center - right - upVec - forward, // Нижняя левая задняя
        center + right - upVec - forward, // Нижняя правая задняя
        center + right + upVec - forward, // Верхняя правая задняя
        center - right + upVec - forward, // Верхняя левая задняя
        center - right - upVec + forward, // Нижняя левая передняя
        center + right - upVec + forward, // Нижняя правая передняя
        center + right + upVec + forward, // Верхняя правая передняя
        center - right + upVec + forward  // Верхняя левая передняя
    };
}


int Cube::GetType() { return FIGURE_CUBE; }



