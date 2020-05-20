#include "extmath.h"

bool isEqual(float a, float b)
{
	return qAbs(a - b) < eps;
}

bool isEqual(QVector3D a, QVector3D b)
{
	return isEqual(a[0], b[0]) && isEqual(a[1], b[1]) && isEqual(a[2], b[2]);
}

bool isnan(QVector2D a)
{
	return qIsNaN(a[0]) || qIsNaN(a[1]);
}

bool isnan(QVector3D a)
{
	return qIsNaN(a[0]) || qIsNaN(a[1]) || qIsNaN(a[2]);
}

bool isnan(QVector4D a)
{
	return qIsNaN(a[0]) || qIsNaN(a[1]) || qIsNaN(a[2]) || qIsNaN(a[3]);
}
