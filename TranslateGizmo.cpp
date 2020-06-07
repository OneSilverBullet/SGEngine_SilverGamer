#include "TranslateGizmo.h"
#include "ModelLoader.h"


TranslateGizmo::TranslateGizmo(QObject* parent /* = 0 */)
{
	setObjectName("Translation Gizmo");
	m_markers.resize(3);

	//加载对应模型并设置对应颜色
	ModelLoader loader;
	m_markers[0] = loader.loadMeshFromFile(":/resources/shapes/TransX.obj");
	m_markers[1] = loader.loadMeshFromFile(":/resources/shapes/TransY.obj");
	m_markers[2] = loader.loadMeshFromFile(":/resources/shapes/TransZ.obj");

	m_markers[0]->material()->setColor(QVector3D(1, 0, 0));
	m_markers[1]->material()->setColor(QVector3D(0, 1, 0));
	m_markers[2]->material()->setColor(QVector3D(0, 0, 1));

	//加载相应的marker
	for (int i = 0; i < m_markers.size(); ++i)
	{
		m_markers[i]->setObjectName("Gizmo Marker");
		m_markers[i]->setParent(this);
	}

	setParent(parent);

}

TranslateGizmo::~TranslateGizmo()
{
}

void TranslateGizmo::translate(QVector3D delta)
{
	if (m_host)
		m_host->translate(delta);
}

//当前Gizmo仅仅负责position
void TranslateGizmo::rotate(QVector3D rotation)
{
}

void TranslateGizmo::rotate(QQuaternion rotation)
{
}

void TranslateGizmo::scale(QVector3D scaling)
{
}

QVector3D TranslateGizmo::position() const
{
	if (m_host)
		return m_host->position();
	return QVector3D(0, 0, 0);
}

QVector3D TranslateGizmo::rotation() const
{
	return QVector3D(0, 0, 0);
}

QVector3D TranslateGizmo::scaling() const
{
	return QVector3D(1,1,1);
}

//全局空间
QMatrix4x4 TranslateGizmo::globalSpaceMatrix() const
{
	QMatrix4x4 model = localModelMatrix();
	if (m_host)
	{
		AbstractEntity* parent = qobject_cast<AbstractEntity*>(m_host->parent());
		while (parent)
		{
			model = parent->globalModelMatrix() *model;
			parent = qobject_cast<AbstractEntity*>(parent->parent());
		}
	}
	return model;
}

//Model Matrix仅仅指rotation矩阵
QMatrix4x4 TranslateGizmo::globalModelMatrix() const
{
	QMatrix4x4 model;
	model.translate(globalSpaceMatrix() * QVector3D(0, 0, 0));
	if (m_host)
	{
		QQuaternion globalRotation;
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

void TranslateGizmo::drag(QPoint from, QPoint to, int scnWidth, int scnHeight, QMatrix4x4 proj, QMatrix4x4 view)
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
		translate(p2 - p1);
	}
	else if (m_axis == Y)
	{
		//设定好方向
		Line y = { QVector3D(0, 0, 0), QVector3D(0, 1, 0) };
		QVector3D p1 = getClosestPointOfLines(y, l1); //计算目标方向与起始线的最近点
		QVector3D p2 = getClosestPointOfLines(y, l2); //计算目标方向与结束线的最近点
		translate(p2 - p1);
	}
	else if (m_axis == Z)
	{
		//设定好方向
		Line z = { QVector3D(0, 0, 0), QVector3D(0, 0, 1) };
		QVector3D p1 = getClosestPointOfLines(z, l1); //计算目标方向与起始线的最近点
		QVector3D p2 = getClosestPointOfLines(z, l2); //计算目标方向与结束线的最近点
		translate(p2 - p1);
	}
}

void TranslateGizmo::setPosition(QVector3D position)
{
	if (m_host)
		m_host->setPosition(position);
}

//当前Gizmo仅仅负责Position。
void TranslateGizmo::setRotation(QQuaternion rotation)
{
}

void TranslateGizmo::setRotation(QVector3D rotation)
{
}

void TranslateGizmo::setScaling(QVector3D scaling)
{
}
