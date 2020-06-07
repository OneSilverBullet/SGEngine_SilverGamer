#include "Camera.h"

Camera::Camera(QObject* parent) : QObject(0)
{

	setObjectName("Camera");
	reset();

	setParent(parent);
}

Camera::Camera(QVector3D position, QVector3D direction, QObject * parent) : QObject(0)
{
	setObjectName("Camera");
	setMovingSpeed(0.1f);
	setFieldOfView(45.0f);
	setAspectRatio(1.0f);
	setNearPlane(0.1f);
	setFarPlane(100000.0f);
	setPosition(position);
	setDirection(direction);
	setParent(parent);
	setUpVector();
}

Camera::Camera(const Camera & camera)
{
	m_movingSpeed = camera.m_movingSpeed;
	m_fieldOfView = camera.m_fieldOfView;
	m_aspectRatio = camera.m_aspectRatio;
	m_position = camera.m_position;
	m_direction = camera.m_direction;
	m_nearPlane = camera.m_nearPlane;
	m_farPlane = camera.m_farPlane;
	setUpVector();
	setObjectName(camera.objectName());

}

Camera::~Camera()
{
}

void Camera::moveForward(float shift)
{
	setPosition(m_position + m_direction * shift * m_movingSpeed);
}

void Camera::moveRight(float shift)
{
	setPosition(m_position + QVector3D::crossProduct(m_direction, m_up) * shift * m_movingSpeed);
}

void Camera::moveUp(float shift)
{
	setPosition(m_position + m_up * shift * m_movingSpeed);
}

void Camera::turnLeft(float angle)
{
	QMatrix4x4 mat;
	mat.rotate(angle, QVector3D(0, 1, 0));
	setDirection(mat *m_direction);
}

void Camera::lookUp(float angle)
{
	QMatrix4x4 mat;
	mat.rotate(angle, QVector3D::crossProduct(m_direction, m_up));
	setDirection(mat* m_direction);
}

void Camera::dumpObjectInfo(int level)
{
}

void Camera::dumpObjectTree(int level)
{
}

QVector3D Camera::position()
{
	return m_position;
}

QVector3D Camera::direction()
{
	return m_direction;
}

float Camera::movingSpeed()
{
	return m_movingSpeed;
}

float Camera::fieldOfView()
{
	return m_fieldOfView;
}

float Camera::aspectRatio()
{
	return m_aspectRatio;
}

float Camera::nearPlane()
{
	return m_nearPlane;
}

float Camera::farPlane()
{
	return m_farPlane;
}

QMatrix4x4 Camera::projectionMatrix() const
{
	QMatrix4x4 mat;
	mat.perspective(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
	return mat;
}

QMatrix4x4 Camera::viewMatrix() const
{
	QMatrix4x4 mat;
	mat.lookAt(m_position, m_position + m_direction, m_up);
	return mat;
}

void Camera::reset()
{
	setMovingSpeed(0.1f);
	setFieldOfView(45.0f);
	setNearPlane(0.1f);
	setAspectRatio(1.0f);
	setFarPlane(100000.0f);
	setPosition(QVector3D(40, 40, 40));
	setDirection(QVector3D(-1, -1, -1));
	setUpVector();
}

void Camera::setMovingSpeed(float movingspeed)
{
	if (!isEqual(m_movingSpeed, movingspeed))
	{
		m_movingSpeed = movingspeed;
		movingSpeedChanged(m_movingSpeed);
	}
}

void Camera::setFieldOfView(float fieldofview)
{
	if (!isEqual(m_fieldOfView, fieldofview))
	{
		m_fieldOfView = fieldofview;
		fieldOfViewChanged(m_fieldOfView);
	}
}

void Camera::setAspectRatio(float aspectratio)
{
	if (!isEqual(m_aspectRatio, aspectratio))
	{
		m_aspectRatio = aspectratio;
		aspectRatioChanged(m_aspectRatio);
	}
}

void Camera::setNearPlane(float nearplane)
{
	if (!isEqual(m_nearPlane, nearplane))
	{
		m_nearPlane = nearplane;
		nearPlaneChanged(m_nearPlane);
	}
}

void Camera::setFarPlane(float farplane)
{
	if (!isEqual(m_farPlane, farplane))
	{
		m_farPlane = farplane;
		farPlaneChanged(m_farPlane);
	}
}

void Camera::setPosition(QVector3D pos)
{
	if (!isEqual(m_position, pos))
	{
		m_position = pos;
		positionChanged(m_position);
	}
}

void Camera::setDirection(QVector3D dir)
{
	if (!isEqual(m_direction, dir))
	{
		m_direction = dir;
		directionChanged(m_direction);
	}
}

void Camera::setUpVector()
{
	QVector3D t = QVector3D::crossProduct(m_direction, QVector3D(0, 1, 0));
	m_up = QVector3D::crossProduct(t, m_direction);
}
