#include "Gridline.h"


GridLine::GridLine(QObject* parent /* = 0 */) : QObject(0)
{
	m_marker = new Mesh(Mesh::Line, this);
	m_marker->setObjectName("Gridline Marker");
	m_marker->setMaterial(new Material);

	setObjectName("Untitled GridLine");
	reset();

	setParent(parent);
}

GridLine::GridLine(const GridLine & gridline) : QObject(0)
{
	m_marker = new Mesh(Mesh::Line, 0);
	m_marker->setMaterial(new Material);
	m_xRange = gridline.m_xRange;
	m_yRange = gridline.m_yRange;
	m_zRange = gridline.m_zRange;
	m_xStride = gridline.m_xStride;
	m_yStride = gridline.m_yStride;
	m_zStride = gridline.m_zStride;
	m_color = gridline.m_color;

	update();
	setObjectName(gridline.objectName());

}

GridLine::~GridLine()
{
}

void GridLine::dumpObjectInfo(int level)
{
}

void GridLine::dumpObjectTree(int level)
{
}

QPair<float, float> GridLine::xRange() const
{
	return m_xRange;
}

QPair<float, float> GridLine::yRange() const
{
	return m_yRange;
}

QPair<float, float> GridLine::zRange() const
{
	return m_zRange;
}

float GridLine::xStride() const
{
	return m_xStride;
}

float GridLine::yStride() const
{
	return m_yStride;
}

float GridLine::zStride() const
{
	return m_zStride;
}

QVector3D GridLine::color() const
{
	return m_color;
}

Mesh * GridLine::marker()
{
	return m_marker;
}

void GridLine::reset()
{
	m_xRange = { -20, 20 };
	m_yRange = { 0, 0 };
	m_zRange = { -20, 20 };
	m_xStride = m_yStride = m_zStride = 1;
	m_color = QVector3D(0.4f, 0.4f, 0.4f);
	update();
}

void GridLine::setXArguments(QVector3D xargs)
{
	if (!isEqual(m_xRange.first, xargs[0]) || !isEqual(m_xRange.second, xargs[1])
		|| !isEqual(m_xStride, xargs[2]))
	{
		m_xRange = { xargs[0], xargs[1] };
		m_xStride = xargs[2];
		update();
		xArgumentsChanged(xargs);
	}
}

void GridLine::setYArguments(QVector3D yargs)
{
	if (!isEqual(m_yRange.first, yargs[0]) || !isEqual(m_yRange.second, yargs[1])
		|| !isEqual(m_yStride, yargs[2]))
	{
		m_yRange = { yargs[0], yargs[1] };
		m_yStride = yargs[2];
		update();
		xArgumentsChanged(yargs);
	}
}

void GridLine::setZArguments(QVector3D zargs)
{
	if (!isEqual(m_zRange.first, zargs[0]) || !isEqual(m_zRange.second, zargs[1])
		|| !isEqual(m_zStride, zargs[2]))
	{
		m_zRange = { zargs[0], zargs[1] };
		m_zStride = zargs[2];
		update();
		xArgumentsChanged(zargs);
	}
}

void GridLine::update()
{
	//依据相应参数生成grid line
	QVector<Vertex> vertices;
	QVector<uint32_t> indices;
	//y是纵轴
	for (float yValue = m_yRange.first; yValue < m_yRange.second + 0.01f; yValue += m_yStride)
	{
		for (float xValue = m_xRange.first; xValue < m_xRange.second + 0.01f; xValue += m_xStride)
		{
			//构造一条直线
			vertices.push_back(Vertex(QVector3D(xValue, yValue, m_zRange.first)));
			vertices.push_back(Vertex(QVector3D(xValue, yValue, m_zRange.second)));
			indices.push_back((uint32_t)vertices.size() - 2);
			indices.push_back((uint32_t)vertices.size() - 1);
		}

		for (float zValue = m_zRange.first; zValue < m_zRange.second + 0.01f; zValue += m_zStride)
		{
			//构造z方向的一条直线
			vertices.push_back(Vertex(QVector3D(m_xRange.first, yValue, zValue)));
			vertices.push_back(Vertex(QVector3D(m_xRange.second, yValue, zValue)));
			indices.push_back((uint32_t)vertices.size() - 2);
			indices.push_back((uint32_t)vertices.size() - 1);
		}
	}

	//设置相应的几何结构与材质
	m_marker->setGeometry(vertices, indices);
	m_marker->material()->setColor(m_color);
}
