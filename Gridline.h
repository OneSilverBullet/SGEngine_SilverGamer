#pragma once

#include <Mesh.h>

class GridLine : public QObject {
	Q_OBJECT

public:
	GridLine(QObject* parent = 0);
	GridLine(const GridLine& gridline);
	~GridLine();

	void dumpObjectInfo(int level = 0);
	void dumpObjectTree(int level = 0);

	QPair<float, float> xRange() const;
	QPair<float, float> yRange() const;
	QPair<float, float> zRange() const;
	float xStride() const;
	float yStride() const;
	float zStride() const;
	QVector3D color() const;
	Mesh* marker();

public slots:
	void reset();
	void setXArguments(QVector3D xargs);
	void setYArguments(QVector3D yargs);
	void setZArguments(QVector3D zargs);

signals:
	void xArgumentsChanged(QVector3D xargs);
	void yArgumentsChanged(QVector3D yargs);
	void zArgumentsChanged(QVector3D zargs);

private:
	QPair<float, float>m_xRange, m_yRange, m_zRange;
	float m_xStride, m_yStride, m_zStride;
	QVector3D m_color;
	Mesh* m_marker;

	void update();
};