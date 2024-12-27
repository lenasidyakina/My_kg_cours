//
// Created by Ivan Sidyakin on 08.11.2024.
//

#ifndef CURS_FIGURE_H
#define CURS_FIGURE_H

#include <QString>
#include <QVector3D>
#include <qcolor.h>
#include <qgenericmatrix.h>
#include "CurTypes.h"

class Poly : public std::vector<QVector3D>
{
    using Base = std::vector<QVector3D>;
public:
    using Base::Base;
    void set_color(const QColor &color) { m_color = color; }
    QColor get_color() const { return m_color; }

    std::vector<QVector3D> get_intersections(int y) const
    {
        std::vector<QVector3D> intersections;
        for (size_t i = 0; i < size(); i++)
        {
            const size_t j = (i + 1) % size();
            if ((((*this)[i].y() <= y && (*this)[j].y() >= y) || ((*this)[i].y() > y && (*this)[j].y() <= y)) && ((*this)[i].y() != (*this)[j].y()))
            {
                const float _y = (y - (*this)[i].y()) / ((*this)[j].y() - (*this)[i].y());
                const float x = (*this)[i].x() + _y * ((*this)[j].x() - (*this)[i].x());
                const float z = (*this)[i].z() + _y * ((*this)[j].z() - (*this)[i].z());

                intersections.push_back(QVector3D(x, y, z));
            }
        }
        std::sort(std::begin(intersections), std::end(intersections), [](const auto &a, const auto &b) {return a.x() < b.x(); });
        return intersections;
    }

    void for_each_pixel(const std::function<void(int, int, float)> &f) const
    {
        const auto [ymin, ymax] = std::minmax_element(begin(), end(), [](const auto &a, const auto &b) {return a.y() < b.y(); });
        for (int y = ymin->y(); y < ymax->y() + 1; y++)
        {
            const auto intersections = get_intersections(y);
            if (intersections.size() != 2)
                continue;
            const auto &p1 = intersections[0];
            const auto &p2 = intersections[1];
            for (int x = static_cast<int>(std::ceil(p1.x())); x <= static_cast<int>(std::floor(p2.x())); x++)
            {
                const float z = ((x - p1.x()) * (p2.z() - p1.z())) / (p2.x() - p1.x()) + p1.z();
                std::invoke(f, x, y, z);
            }
        }
    }

    std::optional<QVector3D> intersect(const QVector3D &rayOrigin, const QVector3D &rayDirection) const
    {
        if (this->size() == 3) {
            return intersect_triangle(rayOrigin, rayDirection, (*this)[0], (*this)[1], (*this)[2]);
        }

        if (this->size() == 4) {
            auto intersection1 = intersect_triangle(rayOrigin, rayDirection, (*this)[0], (*this)[1], (*this)[2]);
            if (intersection1.has_value())
                return intersection1;

            return intersect_triangle(rayOrigin, rayDirection, (*this)[0], (*this)[2], (*this)[3]);

        }

        return std::nullopt;

    }

    QVector3D normal(bool reverse=false) const
    {
        QVector3D result{ 0, 0, 0 };
        if (size() == 3)
        {
            const auto U = this->at(1) - this->at(0);
            const auto V = this->at(2) - this->at(0);

            result.setX((U.y() * V.z()) - (U.z() * V.y()));
            result.setY((U.z() * V.x()) - (U.x() * V.z()));
            result.setZ((U.x() * V.y()) - (U.y() * V.x()));
            return result;
        }
        else if (size() == 4)
        {
            result = QVector3D::crossProduct(this->at(0) - this->at(2), this->at(1) - this->at(3)).normalized();
        }
        return std::pow(-1, int(reverse)) * result;
    }
private:
    QColor m_color{100, 100, 0};

private:
    static std::optional<QVector3D> intersect_triangle(const QVector3D &rayOrigin, const QVector3D &rayDirection,
                                                       const QVector3D &v0, const QVector3D &v1, const QVector3D &v2) {
        const float EPSILON = 1e-6;

        QVector3D edge1 = v1 - v0;
        QVector3D edge2 = v2 - v0;

        QVector3D h = QVector3D::crossProduct(rayDirection, edge2);
        float det = QVector3D::dotProduct(edge1, h);

        if (std::fabs(det) < EPSILON) {
            return std::nullopt;
        }

        float invDet = 1.0f / det;

        QVector3D s = rayOrigin - v0;
        float u = QVector3D::dotProduct(s, h) * invDet;
        if (u < 0.0f || u > 1.0f) {
            return std::nullopt;
        }

        QVector3D q = QVector3D::crossProduct(s, edge1);
        float v = QVector3D::dotProduct(rayDirection, q) * invDet;
        if (v < 0.0f || u + v > 1.0f) {
            return std::nullopt;
        }

        float t = QVector3D::dotProduct(edge2, q) * invDet;
        if (t > EPSILON) {
            return rayOrigin + rayDirection * t;
        }

        return std::nullopt;
    }

};

class Figure {

public:
    int color;
    int x, y, z;
    int trainPosition;

    Figure(QString color) {
        this->color = ColorToInt(color);
    }

    virtual void Draw() {};
    virtual QString getName() { return ""; };
    virtual int GetType() { return FIGURE_UNDEFINED; }
    static uint ColorToInt(const QString color)
    {
        bool ok;
        uint c =  color.mid(1).toUInt(&ok, 16);
        if (!ok)
            c = QString(DEFAULT_FIGURE_COLOR).mid(1).toUInt(&ok, 16);
        return c;
    }

};


#endif //CURS_FIGURE_H



#if 0

//
// Created by Ivan Sidyakin on 08.11.2024.
//

#ifndef CURS_FIGURE_H
#define CURS_FIGURE_H

#include <QString>
#include "CurTypes.h"


class Figure {

public:
    int color;
    int x, y, z;
    int trainPosition;

    Figure(QString color) {
        this->color = ColorToInt(color);
    }

    virtual void Draw() {};
    virtual QString getName() { return ""; };
    virtual int GetType() { return FIGURE_UNDEFINED; }
    static uint ColorToInt(const QString color)
    {
        bool ok;
        uint c =  color.mid(1).toUInt(&ok, 16);
        if (!ok)
            c = QString(DEFAULT_FIGURE_COLOR).mid(1).toUInt(&ok, 16);
        return c;
    }


};


#endif //CURS_FIGURE_H
#endif