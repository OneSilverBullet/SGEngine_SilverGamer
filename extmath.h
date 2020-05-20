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

