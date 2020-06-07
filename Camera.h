#pragma once

#include <Common.h>

class Camera :public QObject {
	Q_OBJECT

public:
	Camera(QObject* parent = 0);
	Camera(QVector3D position, QVector3D direction, QObject* parent=0);
	Camera(const Camera& camera);
	~Camera();

	//ÒÆ¶¯
	void moveForward(float shift);
	void moveRight(float shift);
	void moveUp(float shift);
	//×ª½Ç¶È
	void turnLeft(float angle);
	void lookUp(float angle);

	void dumpObjectInfo(int level = 0);
	void dumpObjectTree(int level = 0);

	QVector3D position();
	QVector3D direction();

	float movingSpeed();
	float fieldOfView();
	float aspectRatio();
	float nearPlane();
	float farPlane();

	QMatrix4x4 projectionMatrix() const;
	QMatrix4x4 viewMatrix() const;

public slots:
	void reset();
	void setMovingSpeed(float movingspeed);
	void setFieldOfView(float fieldofview);
	void setAspectRatio(float aspectratio);
	void setNearPlane(float nearplane);
	void setFarPlane(float farplane);
	void setPosition(QVector3D pos);
	void setDirection(QVector3D dir);

signals:
	void movingSpeedChanged(float value);
	void fieldOfViewChanged(float fieldofview);
	void aspectRatioChanged(float aspectratio);
	void nearPlaneChanged(float nearplane);
	void farPlaneChanged(float farplane);
	void positionChanged(QVector3D position);
	void directionChanged(QVector3D direction);

private:
	float m_movingSpeed, m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane;
	QVector3D m_position, m_direction, m_up;

	void setUpVector();
};