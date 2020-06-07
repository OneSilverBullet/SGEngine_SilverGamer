#include "RotateGizmo.h"
#include "ModelLoader.h"


RotateGizmo(QObject* parent /* = 0 */) : AbstractGizmo(0)
{
	setObjectName("Rotation Gizmo");
	m_markers.resize(3);

	//���ض�Ӧģ�Ͳ����ö�Ӧ��ɫ
	ModelLoader loader;
	m_markers[0] = loader.loadMeshFromFile(":/resources/shapes/RotX.obj");
	m_markers[1] = loader.loadMeshFromFile(":/resources/shapes/RotY.obj");
	m_markers[2] = loader.loadMeshFromFile(":/resources/shapes/RotZ.obj");

	m_markers[0]->material()->setColor(QVector3D(1, 0, 0));
	m_markers[1]->material()->setColor(QVector3D(0, 1, 0));
	m_markers[2]->material()->setColor(QVector3D(0, 0, 1));

	//������Ӧ��marker
	for (int i = 0; i < m_markers.size(); ++i)
	{
		m_markers[i]->setObjectName("Gizmo Marker");
		m_markers[i]->setParent(this);
	}

	setParent(parent);

}

RotateGizmo::~RotateGizmo()
{
}

void RotateGizmo::translate(QVector3D delta)
{
	//��������λ��
}

void RotateGizmo::rotate(QQuaternion rotation)
{
	if (m_host)
	{
		m_host->rotate(rotation);
	}
}

void RotateGizmo::rotate(QVector3D rotation)
{
	if (m_host)
		m_host->rotate(rotation);
}

void RotateGizmo::scale(QVector3D scaling)
{
	//���ǵ�ǰGizmo����Ĺ���
}

QVector3D RotateGizmo::position() const
{
	if (m_host)
		return m_host->position();
	return QVector3D(0, 0, 0);
}

QVector3D RotateGizmo::rotation() const
{
	if (m_host)
		m_host->rotation();
	return QVector3D(0, 0, 0);
}

QVector3D RotateGizmo::scaling() const
{
	return QVector3D(1, 1, 1);
}

QMatrix4x4 RotateGizmo::globalSpaceMatrix() const
{
	QMatrix4x4 model = localModelMatrix();
	if (m_host)
	{
		AbstractEntity* parent = qobject_cast<AbstractEntity*>(m_host->parent());
		while (parent)
		{
			model = parent->globalModelMatrix() * model;
			parent = qobject_cast<AbstractEntity*>(parent->parent());
		}
	}
	return model;
}

QMatrix4x4 RotateGizmo::globalModelMatrix() const
{
	QMatrix4x4 model;
	model.translate(globalSpaceMatrix() * QVector3D(0, 0, 0));

	if (m_host)
	{
		QQuaternion globalRotation = QQuaternion::fromEulerAngles(rotation());
		AbstractEntity* parent = qobject_cast<AbstractEntity*>(m_host->parent());
		while (parent)
		{
			globalRotation = QQuaternion::fromEulerAngles(parent->rotation()) * globalRotation;
			parent = qobject_cast<AbstractEntity*> (parent->parent());
		}
		model.rotate(globalRotation);
	}
	return model;
}


//�ж�rotate�ķ���
//1. �������l1,l2����Ӧ������Ľ���
//2. ����������㵽ԭ������֮���γɵĽǶ�
//3. �ж��������㵽
void RotateGizmo::drag(QPoint from, QPoint to, int scnWidth, int scnHeight, QMatrix4x4 proj, QMatrix4x4 view)
{
	if (m_host == 0) return;
	Line l1 = screenPosToWorldRay(QVector2D(from), QVector2D(scnWidth, scnHeight), proj, view);
	Line l2 = screenPosToWorldRay(QVector2D(to), QVector2D(scnWidth, scnHeight), proj, view);
	QMatrix4x4 invModelMat = globalSpaceMatrix().inverted();
	l1 = invModelMat * l1;
	l2 = invModelMat * l2;
	if (m_axis == X)
	{
		QVector3D p1 = getIntersectionOfLinePlane(l1, QVector3D(0, 0, 0), QVector3D(1, 0, 0));
		QVector3D p2 = getIntersectionOfLinePlane(l2, QVector3D(0, 0, 0), QVector3D(1, 0, 0));
		float theta = qAcos(qMin(qMax(QVector3D::dotProduct(p1, p2) / p1.length() / p2.length(), -1.0), 1.0));
		if (QVector3D::dotProduct(QVector3D(1, 0, 0), QVector3D::crossProduct(p1, p2)) < 0)
		{
			theta = -theta;
		}
		rotate(theta *QVector3D(180.0f / 3.1415926535f, 0, 0));
	}
	else if (m_axis == Y)
	{
		QVector3D p1 = getIntersectionOfLinePlane(l1, QVector3D(0, 0, 0), QVector3D(0, 1, 0));
		QVector3D p2 = getIntersectionOfLinePlane(l2, QVector3D(0, 0, 0), QVector3D(0, 1, 0));
		float theta = qAcos(qMin(qMax(QVector3D::dotProduct(p1, p2) / p1.length() / p2.length(), -1.0), 1.0));
		if (QVector3D::dotProduct(QVector3D(0, 1, 0), QVector3D::crossProduct(p1, p2)) < 0)
		{
			theta = -theta;
		}
		rotate(theta *QVector3D(0, 180.0f / 3.1415926535f, 0));
	}
	else if (m_axis == Z)
	{
		QVector3D p1 = getIntersectionOfLinePlane(l1, QVector3D(0, 0, 0), QVector3D(0, 0, 1));
		QVector3D p2 = getIntersectionOfLinePlane(l2, QVector3D(0, 0, 0), QVector3D(0, 0, 1));
		float theta = qAcos(qMin(qMax(QVector3D::dotProduct(p1, p2) / p1.length() / p2.length(), -1.0), 1.0));
		if (QVector3D::dotProduct(QVector3D(0, 1, 0), QVector3D::crossProduct(p1, p2)) < 0)
		{
			theta = -theta;
		}
		rotate(theta *QVector3D(0.0f, 0.0f, 180.0f / 3.1415926535f));
	}
}

void RotateGizmo::setPosition(QVector3D position)
{
	//���ǵ�ǰ���ְ��
}

void RotateGizmo::setRotation(QVector3D rotation)
{
	if (m_host)
		m_host->setRotation(rotation);
}

void RotateGizmo::setRotation(QQuaternion rotation)
{
	if (m_host)
		m_host->setRotation(rotation);
}

void RotateGizmo::setScaling(QVector3D scaling)
{
	//���ǵ�ǰ��ְ��
}
