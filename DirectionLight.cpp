#include "DirectionLight.h"


DirectionLight::DirectionLight(QObject* parent /* = 0 */) : AbstractLight()
{
	m_color = QVector3D(1.0F, 1.0F, 1.0F);
	m_direction = QVector3D(-1.0f, -1.0f, -1.0f);
	setObjectName("Untitled Direction Light");
	setParent(parent);
}


DirectionLight::DirectionLight(QVector3D color, QVector3D direction, QObject * parent) :AbstractLight(color)
{
	m_direction = direction;
	setObjectName("Untitled Direction Light");
	setParent(parent);
}

DirectionLight::DirectionLight(const DirectionLight & light):AbstractLight(light)
{
	m_direction = light.m_direction;
	setObjectName(light.objectName());
}

DirectionLight::~DirectionLight()
{
}

void DirectionLight::dumpObjectInfo(int level)
{
}

void DirectionLight::dumpObjectTree(int level)
{
}

QVector3D DirectionLight::direction()
{
	return m_direction;
}

void DirectionLight::setDirection(QVector3D direction)
{
	if (!isEqual(direction, m_direction))
	{
		m_direction = direction;
		directionChanged(m_direction);
	}
}
