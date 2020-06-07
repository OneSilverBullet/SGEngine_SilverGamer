#include "Model.h"

Model::Model(QObject* parent = 0):AbstractEntity(0)
{
	setObjectName("Untitled model");
	setParent(parent);
}

Model::Model(const Model & model) : AbstractEntity(model)
{
	for (int i = 0; i < model.m_childMeshes.size(); ++i)
	{
		addChildMesh(new Mesh(*model.m_childMeshes[i]));
	}
	for (int i = 0; i < model.m_childModels.size(); ++i)
	{
		addChildModel(new Model(*model.m_childModels[i]));
	}
}

Model::~Model()
{
}

bool Model::addChildMesh(Mesh * mesh)
{
	if (!mesh || m_childMeshes.contains(mesh))
	{
		return false;
	}
	m_childMeshes.push_back(mesh);
	mesh->setParent(this);
	childMeshAdded(mesh);
	return true;
}

bool Model::addChildModel(Model * model)
{
	if (!model || m_childModels.contains(model))
	{
		return false;
	}
	m_childModels.push_back(model);
	model->setParent(model);
	childModelAdded(model);
	return true;
}

bool Model::removeChildMesh(QObject * mesh, bool recursives)
{
	for (int i = 0; i < m_childMeshes.size(); ++i)
	{
		if (m_childMeshes[i] == mesh)
		{
			m_childMeshes.erase(m_childMeshes.begin() + i);
			if (m_childMeshes.size() == 0 && m_childModels.size() == 0)
			{
				delete this;
			}
			return true;
		}
	}
	if (!recursives)
	{
		return false;
	}
	//如果是递归检查，那么递归检查其child mesh
	for (int i = 0; i < m_childModels.size(); ++i)
	{
		if (m_childModels[i]->removeChildMesh(mesh, recursives))
		{
			return true;
		}
	}
	return false;
}

bool Model::removeChildModel(QObject * model, bool recursive)
{
	for (int i = 0; i < m_childModels.size(); ++i)
	{
		if (m_childModels[i] == model)
		{
			m_childModels.erase(m_childModels.begin() + i);
			childModelRemoved(model);
			if (m_childMeshes.size() == 0 && m_childModels.size() == 0)
			{
				delete this;
			}
			return true;
		}
	}
	if (!recursive)
	{
		return false;
	}
	for (int i = 0; i < m_childModels.size(); ++i)
	{
		if (m_childModels[i]->removeChildModel(model, recursive))
		{
			return true;
		}
	}
	return false;
}

bool Model::isGizmo() const
{
	return false;
}

bool Model::isLight() const
{
	return false;
}

bool Model::isMesh() const
{
	return false;
}

bool Model::isModel() const
{
	return true;
}

QVector3D Model::centerOfMass() const
{
	QVector3D centerOfMass;
	float totalMass = 0;
	for (int i = 0; i < m_childMeshes.size(); ++i)
	{
		centerOfMass += m_childMeshes[i]->centerOfMass() * m_childMeshes[i]->mass();
		totalMass += m_childMeshes[i]->mass();
	}
	for (int i = 0; i < m_childModels.size(); ++i)
	{
		centerOfMass += m_childModels[i]->centerOfMass() * m_childModels[i]->mass();
		totalMass += m_childModels[i]->mass();
	}
	return centerOfMass/totalMass;
}

float Model::mass() const
{
	float totalMass = 0;
	for (int i = 0; i < m_childMeshes.size(); ++i)
	{
		totalMass += m_childMeshes[i]->mass();
	}
	for (int i = 0; i < m_childModels.size(); ++i)
	{
		totalMass += m_childModels[i]->mass();
	}
	return totalMass;
}

//将子面片都合并
Mesh * Model::assemble() const
{
	Mesh* assembledMesh = 0;
	for (int i = 0; i < m_childMeshes.size(); ++i)
	{
		Mesh* old = assembledMesh;
		assembledMesh = Mesh::merge(old, m_childMeshes[i]);
		if (old)
		{
			delete old;
		}
	}
	for (int i = 0; i < m_childModels.size(); ++i)
	{
		Mesh* old1 = assembledMesh;
		Mesh* old2 = m_childModels[i]->assemble();
		assembledMesh = Mesh::merge(old1, old2);
		if (old1) delete old1;
		if (old2) delete old2;
	}
	return assembledMesh;
}

const QVector<Mesh*>& Model::childMeshes() const
{
	return m_childMeshes;
}

const QVector<Model*>& Model::childModels() const
{
	return m_childModels;
}

void Model::reverseNormals()
{
	for (int i = 0; i < m_childMeshes.size(); ++i)
	{
		m_childMeshes[i]->reverseNormals();
	}
	for (int i = 0; i < m_childModels.size(); ++i)
	{
		m_childModels[i]->reverseNormals();
	}
}

void Model::reverseTangents()
{
	for (int i = 0; i < m_childMeshes.size(); ++i)
	{
		m_childMeshes[i]->reverseTangents();
	}
	for (int i = 0; i < m_childModels.size(); ++i)
	{
		m_childModels[i]->reverseTangents();
	}
}

void Model::reverseBitangents()
{
	for (int i = 0; i < m_childMeshes.size(); ++i)
	{
		m_childMeshes[i]->reverseBitangents();
	}
	for (int i = 0; i < m_childModels.size(); ++i)
	{
		m_childModels[i]->reverseBitangents();
	}
}

void Model::childEvent(QChildEvent * event)
{
	if (event->added())
	{
		if (Model* model = qobject_cast<Model*>(event->child()))
		{
			addChildModel(model);
		}
		else if (Mesh* mesh = qobject_cast<Mesh*>(event->child()))
		{
			addChildMesh(mesh);
		}
	}
	else if (event->removed())
	{
		if (removeChildModel(event->child(), false)) return;
		if (removeChildMesh(event->child(), false)) return;
	}
}
