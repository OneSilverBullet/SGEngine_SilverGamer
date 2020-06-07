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

Line operator*(const QMatrix4x4 & m, const Line & l)
{
	return Line();
}

QVector3D getIntersectionOfLinePlane(Line l, Plane p)
{
	float t = QVector3D::dotProduct(p.n, p.v - l.st) / QVector3D::dotProduct(p.n, l.dir);
	if (isnan(t))
	{
		//打印错误信息
	}
	return l.st + l.dir * t; //调用方程计算值
}

QVector3D getClosestPointOfLines(Line l1, Line l2)
{
	l1.dir.normalize();
	l2.dir.normalize();
	QVector3D n = QVector3D::crossProduct(l1.dir, l2.dir);
	if (isnan(n))
	{
		//打印错误信息
	}

	return getIntersectionOfLinePlane({ l1.st, l1.dir }, 
		{l2.st, QVector3D::crossProduct(n, l2.dir)});

}

Line screenPosToWorldRay(QVector2D cursorPos, QVector2D windowSize, QMatrix4x4 proj, QMatrix4x4 view)
{
	cursorPos.setY(windowSize.y() - cursorPos.y());
	QMatrix4x4 inv = (proj * view).inverted(); //光栅化的逆运算
	QVector4D st((cursorPos / windowSize - QVector2D(0.5, 0.5))*2.0f, -1.0f, 1.0f);
	QVector4D ed((cursorPos / windowSize - QVector2D(0.5, 0.5))*2.0f, 0.0, 1.0f);

	st = inv * st;
	st /= st.w();
	ed = inv * ed;
	ed /= ed.w();
	if (isnan(st))
	{

	}
	if (isnan(ed))
	{

	}
	QVector3D dir = (ed - st).toVector3D();
	if (isnan(dir))
	{

	}
	return Line{st.toVector3D(), dir.normalized()};
}
