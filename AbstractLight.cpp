#include "AbstractLight.h"


AbstractLight::AbstractLight() :QObject(0)
{
	m_color = QVector3D(1.0, 1.0, 1.0);
	m_enabled = true;
	m_intensity = 1.0f;
	setObjectName("Untitled Light");
}

AbstractLight::AbstractLight(QVector3D color)
{
	m_color = color;
	m_enabled = true;
	m_intensity = 1.0f;
	setObjectName("Untitled Light");
}

AbstractLight::AbstractLight(const AbstractLight & light)
{
	m_color = light.m_color;
	m_enabled = light.m_enabled;
	m_intensity = light.m_intensity;
	setObjectName(light.objectName());
}

AbstractLight::~AbstractLight()
{
}

QVector3D AbstractLight::color()
{
	return m_color;
}

bool AbstractLight::enabled()
{
	return m_enabled;
}

float AbstractLight::intensity()
{
	return m_intensity;
}

Mesh * AbstractLight::marker() const
{
	return nullptr;
}

void AbstractLight::setColor(QVector3D color)
{
	if (!isEqual(m_color, color))
	{
		m_color = color;
		colorChanged(m_color);
	}
}

void AbstractLight::setEnabled(bool enabled)
{
	if (m_enabled != enabled)
	{
		m_enabled = enabled;
		enabledChanged(m_enabled);
	}
}

void AbstractLight::setIntensity(float intensity)
{
	if (!isEqual(m_intensity, intensity))
	{
		m_intensity = intensity;
		intensityChanged(m_intensity);
	}
}
