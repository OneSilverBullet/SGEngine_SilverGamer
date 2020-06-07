#pragma once

#include <QtMath>
#include <QPoint>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QQuaternion>

#define eps (1e-4f)
#define inf (1000000.0f)
#define rad(n) ((n) * M_PI / 180)

bool isEqual(float a, float b);
bool isEqual(QVector3D a, QVector3D b);

bool isnan(QVector2D a);
bool isnan(QVector3D b);
bool isnan(QVector4D a);

struct Line
{
	QVector3D st, dir;
};

struct Plane
{
	QVector3D v, n;
};

Line operator*(const QMatrix4x4& m, const Line& l);

/*
得到线与片段的交点
L = st + dir * t
*/
QVector3D getIntersectionOfLinePlane(Line l, Plane p);

//得到l1和l2点之间的最近点
//L1 = st1 + dir1 * t1
//L2 = st2 + dir2 * t2
QVector3D getClosestPointOfLines(Line l1, Line l2);

//Screen Space 2 world Space
Line screenPosToWorldRay(QVector2D cursorPos, QVector2D windowSize,
	QMatrix4x4 proj, QMatrix4x4 view);