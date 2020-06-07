#include "ScaleGizmo.h"
#include "ModelLoader.h"

ScaleGizmo::ScaleGizmo(QObject* parent /* = 0 */) : AbstractGizmo(0)
{
	setObjectName("Scaling Gizmo");
	m_markers.resize(3);

	//加载对应模型并设置对应颜色
	ModelLoader loader;
	m_markers[0] = loader.loadMeshFromFile(":/resources/shapes/ScaleX.obj");
	m_markers[1] = loader.loadMeshFromFile(":/resources/shapes/ScaleY.obj");
	m_markers[2] = loader.loadMeshFromFile(":/resources/shapes/ScaleZ.obj");

	m_markers[0]->material()->setColor(QVector3D(1, 0, 0));
	m_markers[1]->material()->setColor(QVector3D(0, 1, 0));
	m_markers[2]->material()->setColor(QVector3D(0, 0, 1));

	//加载相应的marker
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
	//translate 功能对scale是禁止的
}

void ScaleGizmo::rotate(QQuaternion rotation)
{
	//rotate 功能对scale是禁止的
}

void ScaleGizmo::rotate(QVector3D rotation)
{
	//rotate 功能对scale是禁止的
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
		//迭代一次进行矩阵乘法，得到最终的全局矩阵
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
	Line l1 = screenPosToWorldRay(QVector2D(from), QVector2D(scnWidth, scnHeight), proj, view); //得到起始点位置
	Line l2 = screenPosToWorldRay(QVector2D(to), QVector2D(scnWidth, scnHeight), proj, view); //得到结束点位置
	QMatrix4x4 invModelMat = globalSpaceMatrix().inverted();
	l1 = invModelMat * l1;
	l2 = invModelMat * l2;
	if (m_axis == X)
	{
		//设定好方向
		Line x = { QVector3D(0, 0, 0), QVector3D(1, 0, 0) };
		QVector3D p1 = getClosestPointOfLines(x, l1); //计算目标方向与起始线的最近点
		QVector3D p2 = getClosestPointOfLines(x, l2); //计算目标方向与结束线的最近点
		scale(QVector3D(p2.x() / p1.x(), 1.0f, 1.0f)); //依据两个最近点的x轴向的除法完成放缩
	}
	else if (m_axis == Y)
	{
		//设定好方向
		Line y = { QVector3D(0, 0, 0), QVector3D(0, 1, 0) };
		QVector3D p1 = getClosestPointOfLines(y, l1); //计算目标方向与起始线的最近点
		QVector3D p2 = getClosestPointOfLines(y, l2); //计算目标方向与结束线的最近点
		scale(QVector3D(1.0, p1.y() / p2.y(), 1.0f)); //依据两个最近点的x轴向的除法完成放缩
	}
	else if (m_axis == Z)
	{
		//设定好方向
		Line z = { QVector3D(0, 0, 0), QVector3D(0, 0, 1) };
		QVector3D p1 = getClosestPointOfLines(z, l1); //计算目标方向与起始线的最近点
		QVector3D p2 = getClosestPointOfLines(z, l2); //计算目标方向与结束线的最近点
		scale(QVector3D(1.0, 1.0f, p1.z() / p2.z())); //依据两个最近点的x轴向的除法完成放缩
	}
}

//由于是Scale Gizmo，Position、Rotation部分都不负责管辖
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
