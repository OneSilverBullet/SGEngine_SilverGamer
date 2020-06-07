#include "AmbientLight.h"


AmbientLight::AmbientLight(QObject* parent) : AbstractLight()
{
	m_color = QVector3D(1.0f, 1.0f, 1.0f);
	setObjectName("Untitled Ambient Light");
	setParent(parent);
}

AmbientLight::AmbientLight(QVector3D color, QObject * parent) : AbstractLight(color)
{
	setObjectName("Untitled Ambient Light");
	setParent(parent);
}


AmbientLight::AmbientLight(const AmbientLight & light) : AbstractLight(light){
	setObjectName(light.objectName());
}

AmbientLight::~AmbientLight()
{

}

void AmbientLight::dumpObjectInfo(int level)
{
}

void AmbientLight::dumpObjectTree(int level)
{
}
