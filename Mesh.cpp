#include "Mesh.h"
#include <AbstractGizmo.h>
#include <AbstractLight.h>


Mesh::Mesh(QObject * parent) : AbstractEntity(0)
{
	m_meshType = Triangle;
	m_material = 0;
	setObjectName("Untitled Mesh");
	setParent(parent);
}

Mesh::Mesh(MeshType meshType, QObject * parent) : AbstractEntity(0)
{
	m_meshType = meshType;
	m_material = 0;
	setObjectName("Untitled Mesh");
	setParent(parent);
}

Mesh::Mesh(const Mesh & mesh)
{
	m_meshType = mesh.m_meshType;
	m_material = mesh.m_material;
	m_indices = mesh.m_indices;
	m_vertices = mesh.m_vertices;
	setObjectName(mesh.objectName());
}

Mesh::~Mesh()
{
}

void Mesh::dumpObjectInfo(int level)
{
}

void Mesh::dumpObjectTree(int level)
{
}

bool Mesh::isGizmo() const
{
	if (qobject_cast<AbstractGizmo*> (parent()))
	{
		return true;
	}
	return false;
}

bool Mesh::isLight() const
{
	if (qobject_cast<AbstractLight*>(parent()))
	{
		return true;
	}
	return false;
}

bool Mesh::isMesh() const
{
	if (isGizmo())return false;
	if (isLight())return false;
	return true;
}

bool Mesh::isModel() const
{
	return false;
}

QVector3D Mesh::centerOfMass() const
{
	QVector3D centerOfMass;
	float totalMass = 0;
	QMatrix4x4 modelMat = globalModelMatrix();
	for (int i = 0; i < m_indices.size(); )
	{
		QVector3D centroid;
		float mass = 0;
		if (m_meshType == Point)
		{
			centroid = modelMat * m_vertices[m_indices[i + 0]].position;
			mass = 1.0f;
			i += 1;
		}
		else if (m_meshType == Line)
		{
			QVector3D p1 = modelMat * m_vertices[m_indices[i + 0]].position;
			QVector3D p2 = modelMat * m_vertices[m_indices[i + 1]].position;
			centroid = (p1 + p2) / 2;
			mass = p1.distanceToPoint(p2);
			i += 2;
		}
		else if (m_meshType == Triangle)
		{
			QVector3D p1 = modelMat * m_vertices[m_indices[i + 0]].position;
			QVector3D p2 = modelMat * m_vertices[m_indices[i + 1]].position;
			QVector3D p3 = modelMat * m_vertices[m_indices[i + 2]].position;
			centroid = (p1 + p2 + p3) / 3;
			mass = QVector3D::crossProduct(p2 - p1, p3 - p1).length() / 2;
			i += 3;
		}
		centerOfMass += centroid * mass;
		totalMass += mass;
	}
	return centerOfMass / totalMass;
}

float Mesh::mass() const
{
	QVector3D centerOfMass;
	float totalMass = 0;
	QMatrix4x4 modelMat = globalModelMatrix();
	for (int i = 0; i < m_indices.size(); )
	{
		float mass = 0;
		if (m_meshType == Point)
		{
			mass = 1.0f;
			i += 1;
		}
		else if (m_meshType == Line)
		{
			QVector3D p1 = modelMat * m_vertices[m_indices[i + 0]].position;
			QVector3D p2 = modelMat * m_vertices[m_indices[i + 1]].position;
			mass = p1.distanceToPoint(p2);
			i += 2;
		}
		else if (m_meshType == Triangle)
		{
			QVector3D p1 = modelMat * m_vertices[m_indices[i + 0]].position;
			QVector3D p2 = modelMat * m_vertices[m_indices[i + 1]].position;
			QVector3D p3 = modelMat * m_vertices[m_indices[i + 2]].position;
			mass = QVector3D::crossProduct(p2 - p1, p3 - p1).length() / 2;
			i += 3;
		}
		totalMass += mass;
	}
	return totalMass;
}

Mesh::MeshType Mesh::meshType() const
{
	return m_meshType;
}

const QVector<Vertex>& Mesh::vertices() const
{
	return m_vertices;
}

const QVector<uint32_t>& Mesh::indices() const
{
	return m_indices;
}

Material * Mesh::material() const
{
	return m_material;
}

Mesh * Mesh::merge(const Mesh * mesh1, const Mesh * mesh2)
{
	if (mesh1 == 0 && mesh2 == 0)
	{
		return 0;
	}
	else if (mesh1 == 0 || mesh2 == 0)
	{
		if (mesh1 == 0) mesh1 = mesh2;
		Mesh* mergeMesh = new Mesh(mesh1->meshType());
		mergeMesh->setObjectName(mesh1->objectName());
		mergeMesh->setMaterial(new Material(*mesh1->material()));
		for (int i = 0; i < mesh1->m_vertices.size(); ++i)
		{
			mergeMesh->m_vertices.push_back(mesh1->globalModelMatrix() * mesh1->m_vertices[i]);
		}
		mergeMesh->m_indices = mesh1->m_indices;
		return mergeMesh;
	}

	if (mesh1->meshType() != mesh2->meshType())
	{

	}

	Mesh* mergeMesh = new Mesh(mesh1->meshType());
	mergeMesh->setObjectName(mesh1->objectName() + mesh2->objectName());
	mergeMesh->setMaterial(new Material);

	for (int i = 0; i < mesh1->m_vertices.size(); ++i)
	{
		mergeMesh->m_vertices.push_back(mesh1->globalModelMatrix() * mesh1->m_vertices[i]);
	}

	for (int i = 0; i < mesh2->m_vertices.size(); ++i)
	{
		mergeMesh->m_vertices.push_back(mesh2->globalModelMatrix() * mesh2->m_vertices[i]);
	}

	mergeMesh->m_indices = mesh1->m_indices;
	for (int i = 0; i < mesh2->m_indices.size(); ++i)
	{
		mergeMesh->m_indices.push_back(mesh2->m_indices[i] + mesh1->m_indices.size());
	}

	return mergeMesh;
}


void Mesh::setMeshType(MeshType meshType)
{
	if (m_meshType != meshType)
	{
		m_meshType = meshType;
		meshTypeChanged(m_meshType);
	}
}

void Mesh::setGeometry(const QVector<Vertex>& vertices, const QVector<uint32_t>& indices)
{
	if (vertices != m_vertices || indices != m_indices)
	{
		m_vertices = vertices;
		m_indices = indices;
		geometryChanged(m_vertices, m_indices);
	}
}

void Mesh::setMaterial(Material * newMaterial)
{
	if (m_material == newMaterial)
		return false;

	if (m_material)
	{
		Material* tmp = m_material;
		m_material = 0;
		delete tmp;
	}

	if (newMaterial)
	{
		m_material = newMaterial;
		m_material.setParent(this);
	}
	materialChanged(m_material);
	return true;
}

void Mesh::reverseNormals()
{
	for (int i = 0; i < m_vertices.size(); ++i)
	{
		m_vertices[i].normal = -m_vertices[i]->normal;
	}
	geometryChanged(m_vertices, m_indices);
}

void Mesh::reverseTangents()
{
	for (int i = 0; i < m_vertices.size(); ++i)
	{
		m_vertices[i].tangent = -m_vertices[i].tangent;
	}
	geometryChanged(m_vertices, m_indices);
}

void Mesh::reverseBitangents()
{
	for (int i = 0; i < m_vertices.size(); ++i)
	{
		m_vertices[i].bitangent = -m_vertices[i].bitangent;
	}
	geometryChanged(m_vertices, m_indices);
}

void Mesh::childEvent(QChildEvent * event)
{
	if (event->added())
	{
		if (Material* material = qobject_cast<Material*>(event->child()))
		{
			setMaterial(material);
		}
		else if (event->removed())
		{
			if (e->child() == m_material)
			{
				m_material = 0;
				materialChanged(0);
			}
		}
	}
}
