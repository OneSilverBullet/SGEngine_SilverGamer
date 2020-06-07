#include "SpotLight.h"

SpotLight::SpotLight(QObject* parent /* = 0 */):AbstractLight()
{
	m_position = QVector3D(0.0f, 0.0f, 0.0f);
	m_direction = QVector3D(0.0f, -1.0f, 0.0f);
	m_innerCutOff = 30.0f;
	m_outerCutOff = 45.0f;
	m_enabledAttenuation = false;
	m_attenuationQuadratic = 0.0007f;
	m_attenuationLinear = 0.014f;
	m_attenuationConstant = 1.0f;
	
	initMarker();
	setObjectName("Untitled Spotlight");
	setParent(parent);
}


SpotLight::SpotLight(QVector3D color, QVector3D position, QVector3D direction, QObject * parent):AbstractLight(color)
{
	m_position = position;
	m_direction = direction;
	m_innerCutOff = 30.0f;
	m_outerCutOff = 45.0f;
	m_enabledAttenuation = false;
	m_attenuationQuadratic = 0.0007f;
	m_attenuationLinear = 0.014f;
	m_attenuationConstant = 1.0f;

	initMarker();
	setObjectName("Untitled Spotlight");
	setParent(parent);
}

SpotLight::SpotLight(const SpotLight & light)
{
	m_position = light.m_position;
	m_direction = light.m_direction;
	m_innerCutOff = light.m_innerCutOff;
	m_outerCutOff = light.m_outerCutOff;
	m_enabledAttenuation = light.m_enabledAttenuation;
	m_attenuationLinear = light.m_attenuationLinear;
	m_attenuationQuadratic = light.m_attenuationQuadratic;
	m_attenuationConstant = light.m_attenuationConstant;
	initMarker();
	setObjectName(light.objectName());
}

SpotLight::~SpotLight()
{
	delete m_marker;
}

void SpotLight::translate(QVector3D delta)
{
	setPosition(m_position + delta);
}

void SpotLight::dumpObjectInfo(int level)
{
}

void SpotLight::dumpObjectTree(int level)
{
}

bool SpotLight::visible() const
{
	return m_marker->visible();
}

QVector3D SpotLight::position() const
{
	return m_position;
}

QVector3D SpotLight::direction() const
{
	return m_direction;
}

float SpotLight::innerCutOff() const
{
	return m_innerCutOff;
}

float SpotLight::outerCutOff() const
{
	return m_outerCutOff;
}

bool SpotLight::enableAttenuation()
{
	return m_enabledAttenuation;
}

QVector3D SpotLight::attenuationArguments() const
{
	return QVector3D(m_attenuationQuadratic, m_attenuationLinear, m_attenuationConstant);
}

float SpotLight::attenuationQuadratic() const
{
	return m_attenuationQuadratic;
}

float SpotLight::attenuationLinear() const
{
	return m_attenuationLinear;
}

float SpotLight::attenuationConstant() const
{
	return m_attenuationConstant;
}

Mesh * SpotLight::marker() const
{
	return m_marker;
}

void SpotLight::setColor(QVector3D color)
{
	AbstractLight::setColor(color);
	m_marker->material()->setColor(color);
}

void SpotLight::setVisible(bool visible)
{
	m_marker->setVisible(visible);
}

void SpotLight::setEnabled(bool enabled)
{
	AbstractLight::setEnabled(enabled);
	m_marker->setVisible(enabled);
}

void SpotLight::setPosition(QVector3D position)
{
	if (!isEqual(position, m_position))
	{
		m_position = position;
		m_marker->setPosition(m_position);
		positionChanged(m_position);
	}
}

void SpotLight::setDirection(QVector3D direction)
{
	if (!isEqual(m_direction, direction))
	{
		m_direction = direction;
		m_marker->setRotation(QQuaternion::rotationTo(QVector3D(0, -1, 0), m_direction));
		directionChanged(m_direction);
	}
}

void SpotLight::setInnerCutOff(float innerCutOff)
{
	if (!isEqual(m_innerCutOff, innerCutOff))
	{
		m_innerCutOff = innerCutOff;
		innerCutOffChanged(m_innerCutOff);
	}
}

void SpotLight::setOuterCutOff(float outerCutOff)
{
	if (!isEqual(m_outerCutOff, outerCutOff))
	{
		m_outerCutOff = outerCutOff;
		outerCutOffChanged(m_outerCutOff);
	}
}

void SpotLight::setEnableAttenuation(bool enabled)
{
	if (m_enabledAttenuation != enabled)
	{
		m_enabledAttenuation = enabled;
		enableAttenuationChanged(m_enabledAttenuation);
	}
}

void SpotLight::setAttenuationArguments(QVector3D value)
{
	setAttenuationQuadratic(value[0]);
	setAttenuationLinear(value[1]);
	setAttenuationConstant(value[2]);
}

void SpotLight::setAttenuationQuadratic(float value)
{
	if (!isEqual(m_attenuationQuadratic, value))
	{
		m_attenuationQuadratic = value;
		attenuationQuadraticChanged(m_attenuationQuadratic);
		attenuationArgumentsChanged(this->attenuationArguments());
	}
}

void SpotLight::setAttenuationLinear(float value)
{
	if (!isEqual(m_attenuationLinear, value))
	{
		m_attenuationLinear = value;
		attenuationLinearChanged(m_attenuationLinear);
		attenuationArgumentsChanged(this->attenuationArguments());
	}
}

void SpotLight::setAttenuationConstant(float value)
{
	if (!isEqual(m_attenuationConstant, value))
	{
		m_attenuationConstant = value;
		attenuationConstantChanged(m_attenuationConstant);
		attenuationArgumentsChanged(this->attenuationArguments());
	}
}

void SpotLight::setDirectionFromRotation(QVector3D rotation)
{
	QVector3D direction = QQuaternion::fromEulerAngles(rotation) * QVector3D(0, -1, 0);
	if (!isEqual(m_direction, direction))
	{
		m_direction = direction;
		directionChanged(m_direction);
	}
}


void SpotLight::initMarker()
{
	


}
