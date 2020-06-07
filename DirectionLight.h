#pragma once

#include <AbstractLight.h>

class DirectionLight : public AbstractLight
{
	Q_OBJECT

public:
	DirectionLight(QObject* parent = 0);
	DirectionLight(QVector3D color, QVector3D direction, QObject* parent = 0);
	DirectionLight(const DirectionLight& light);
	~DirectionLight();

	void dumpObjectInfo(int level = 0) override;
	void dumpObjectTree(int level = 0) override;

	QVector3D direction();


public slots:
	void setDirection(QVector3D direction);

signals:
	void directionChanged(QVector3D direction);

private:
	QVector3D m_direction;




};