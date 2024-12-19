
#include "Cube.h"


#include <QPainter>

void Cube::Draw()
{

}


QString Cube::getName()
{
    return "Куб " + QString("%1px").arg(this->EdgeLength);
}

void Cube::updateVertices(const Point3D& normal, const Point3D& tangent, const Point3D& up, const Point3D& center) {
    float halfEdge = EdgeLength / 2.0f;

    vertices.clear(); // Очищаем старые вершины

    // Первая вершина
    vertices.push_back(center + normal * halfEdge + tangent * halfEdge + up * halfEdge);
    // Вторая вершина
    vertices.push_back(center + normal * halfEdge - tangent * halfEdge + up * halfEdge);
    // Третья вершина
    vertices.push_back(center - normal * halfEdge - tangent * halfEdge + up * halfEdge);
    // Четвертая вершина
    vertices.push_back(center - normal * halfEdge + tangent * halfEdge + up * halfEdge);
    // Пятая вершина
    vertices.push_back(center + normal * halfEdge + tangent * halfEdge - up * halfEdge);
    // Шестая вершина
    vertices.push_back(center + normal * halfEdge - tangent * halfEdge - up * halfEdge);
    // Седьмая вершина
    vertices.push_back(center - normal * halfEdge - tangent * halfEdge - up * halfEdge);
    // Восьмая вершина
    vertices.push_back(center - normal * halfEdge + tangent * halfEdge - up * halfEdge);
}



int Cube::GetType() { return FIGURE_CUBE; }



