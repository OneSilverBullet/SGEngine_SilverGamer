#include "ScaleGizmo.h"
#include "ModelLoader.h"

ScaleGizmo::ScaleGizmo(QObject* parent /* = 0 */) : AbstractGizmo(0)
{
	setObjectName("Scaling Gizmo");
	m_markers.resize(3);

	//���ض�Ӧģ�Ͳ����ö�Ӧ��ɫ
	ModelLoader loader;
	m_markers[0] = loader.loadMeshFromFile(":/resources/shapes/ScaleX.obj");
	m_markers[1] = loader.loadMeshFromFile(":/resources/shapes/ScaleY.obj");
	m_markers[2] = loader.loadMeshFromFile(":/resources/shapes/ScaleZ.obj");

	m_markers[0]->material()->setColor(QVector3D(1, 0, 0));
	m_markers[1]->material()->setColor(QVector3D(0, 1, 0));
	m_markers[2]->material()->setColor(QVector3D(0, 0, 1));

	//������Ӧ��marker
	for (int i=0; i<m_markers.size(); ++i)
	{
		m_markers[i]->setObjectName("Gizmo Marker");
		m_markers[i]->setParent(this);
	}

	setParent(parent);
}

ScaleGizmo::~ScaleGizmo()
{
}

void ScaleGizmo::translate(QVector3D delta)
{
	//translate ���ܶ�scale�ǽ�ֹ��
}

void ScaleGizmo::rotate(QQuaternion rotation)
{
	//rotate ���ܶ�scale�ǽ�ֹ��
}

void ScaleGizmo::rotate(QVector3D rotation)
{
	//rotate ���ܶ�scale�ǽ�ֹ��
}

void ScaleGizmo::scale(QVector3D scaling)
{
	if (m_host)
		m_host->scale(scaling);
}

QVector3D ScaleGizmo::position() const
{
	if (m_host)
	{
		return m_host->position();
	}
	else
		return QVector3D(0, 0, 0);
}

QVector3D ScaleGizmo::rotation() const
{
	if (m_host)
		return m_host->rotation();
	return QVector3D(0, 0, 0);
}

QVector3D ScaleGizmo::scaling() const
{
	if (m_host)
		return m_host->scaling();
	return QVector3D(0, 0, 0);
}

QMatrix4x4 ScaleGizmo::globalSpaceMatrix() const
{
	QMatrix4x4 model = localModelMatrix();
	if (m_host)
	{
		AbstractEntity* parent = qobject_cast<AbstractEntity*>(m_host->parent());
		//����һ�ν��о���˷����õ����յ�ȫ�־���
		while (parent)
		{
			model = parent->globalModelMatrix() * model;
			parent = qobject_cast<AbstractEntity*>(parent->parent());
		}
	}
	return model;
}

QMatrix4x4 ScaleGizmo::globalModelMatrix() const
{
	QMatrix4x4 model;
	model.translate(globalSpaceMatrix() * QVector3D(0, 0, 0));
	if (m_host)
	{
		QQuaternion globalRotation = QQuaternion::fromEulerAngles(rotation()) * globalRotation;
		AbstractEntity* parent = qobject_cast<AbstractEntity*>(m_host->parent());
		while (parent)
		{
			globalRotation = QQuaternion::fromEulerAngles(parent->rotation()) * globalRotation;
			parent = qobject_cast<AbstractEntity*>(parent->parent());
		}
		model.rotate(globalRotation);
	}
	return model;
}

void ScaleGizmo::drag(QPoint from, QPoint to, int scnWidth, int scnHeight, QMatrix4x4 proj, QMatrix4x4 view)
{
	if (m_host == 0) return;
	Line l1 = screenPosToWorldRay(QVector2D(from), QVector2D(scnWidth, scnHeight), proj, view); //�õ���ʼ��λ��
	Line l2 = screenPosToWorldRay(QVector2D(to), QVector2D(scnWidth, scnHeight), proj, view); //�õ�������λ��
	QMatrix4x4 invModelMat = globalSpaceMatrix().inverted();
	l1 = invModelMat * l1;
	l2 = invModelMat * l2;
	if (m_axis == X)
	{
		//�趨�÷���
		Line x = { QVector3D(0, 0, 0), QVector3D(1, 0, 0) };
		QVector3D p1 = getClosestPointOfLines(x, l1); //����Ŀ�귽������ʼ�ߵ������
		QVector3D p2 = getClosestPointOfLines(x, l2); //����Ŀ�귽��������ߵ������
		scale(QVector3D(p2.x() / p1.x(), 1.0f, 1.0f)); //��������������x����ĳ�����ɷ���
	}
	else if (m_axis == Y)
	{
		//�趨�÷���
		Line y = { QVector3D(0, 0, 0), QVector3D(0, 1, 0) };
		QVector3D p1 = getClosestPointOfLines(y, l1); //����Ŀ�귽������ʼ�ߵ������
		QVector3D p2 = getClosestPointOfLines(y, l2); //����Ŀ�귽��������ߵ������
		scale(QVector3D(1.0, p1.y() / p2.y(), 1.0f)); //��������������x����ĳ�����ɷ���
	}
	else if (m_axis == Z)
	{
		//�趨�÷���
		Line z = { QVector3D(0, 0, 0), QVector3D(0, 0, 1) };
		QVector3D p1 = getClosestPointOfLines(z, l1); //����Ŀ�귽������ʼ�ߵ������
		QVector3D p2 = getClosestPointOfLines(z, l2); //����Ŀ�귽��������ߵ������
		scale(QVector3D(1.0, 1.0f, p1.z() / p2.z())); //��������������x����ĳ�����ɷ���
	}
}

//������Scale Gizmo��Position��Rotation���ֶ��������Ͻ
void ScaleGizmo::setPosition(QVector3D position)
{

}

void ScaleGizmo::setRotation(QQuaternion rotation)
{

}

void ScaleGizmo::setRotation(QVector3D rotation)
{
}

void ScaleGizmo::setScaling(QVector3D scaling)
{
	if (m_host)
		m_host->setScaling(scaling);
}
