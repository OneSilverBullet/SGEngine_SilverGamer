#pragma once

#include <AbstractLight.h>

class PointLight : public AbstractLight
{
	Q_OBJECT

public :
	PointLight(QObject* parent = 0);
	PointLight(QVector3D color, QVector3D position, QObject* parent = 0);
	PointLight(const PointLight& light);
	~PointLight();



};
