#include "ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/importerdesc.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ModelLoader::ModelLoader()
{
	m_aiScenePtr = 0;
}

Model * ModelLoader::loadModelFromFile(QString filePath)
{
	if (filePath.length() == 0)
	{
		return 0;
	}

	Assimp::Importer importer;
	unsigned int flags =
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices |  //使用索引获取顶点引用，可以节省内存
		aiProcess_OptimizeGraph |
		aiProcess_GenUVCoords;

	//加载
	if (filePath[0] == ':') //相对路径
	{
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly))
		{
			//不能打开对应的文件，退出
			exit(-1);
		}
		//相对路径，从内存当中加载数据
		QByteArray bytes = file.readAll();
		m_aiScenePtr = importer.ReadFileFromMemory(bytes.constData(), bytes.length(), flags);
	}
	else
	{
		//绝对路径，从硬盘加载数据
		m_dir = QFileInfo(filePath).absoluteDir();
		m_aiScenePtr = importer.ReadFile(filePath.toStdString(), flags);
	}

	//如果模型加载失败
	if (!m_aiScenePtr || !m_aiScenePtr->mRootNode || m_aiScenePtr->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
	{
		m_log += importer.GetErrorString();
		return 0;
	}

	//依据加载好的m_Scene进行模型加载
	Model* model = loadModel(m_aiScenePtr->mRootNode);
	model->setObjectName(QFileInfo(filePath).baseName());
	return model;
}

Mesh * ModelLoader::loadMeshFromFile(QString filePath)
{
	Model* model = loadModelFromFile(filePath);
	Mesh* assembledMesh = model->assemble();
	delete model;
	return assembledMesh;
}

Model * ModelLoader::loadConeModel()
{
	//该loader只针对于Cone
	static ModelLoader* loader = new ModelLoader;
	Model* model = loader->loadModelFromFile(":/resources/shapes/Cone.obj");
	return model;
}

Model * ModelLoader::loadCubeModel()
{
	static ModelLoader* loader = new ModelLoader;
	Model* model = loader->loadModelFromFile(":/resources/shapes/Cube.obj");
	return model;
}

Model * ModelLoader::loadCylinderModel()
{
	static ModelLoader* loader = new ModelLoader;
	Model* model = loader->loadModelFromFile(":/resources/shapes/Cylinder.obj");
	return model;
}

Model * ModelLoader::loadPlaneModel()
{
	static ModelLoader* loader = new ModelLoader;
	Model* model = loader->loadModelFromFile(":/resources/shapes/Plane.obj");
	return model;
}

Model * ModelLoader::loadSphereModel()
{
	static ModelLoader* loader = new ModelLoader;
	Model* model = loader->loadModelFromFile(":/resources/shapes/Sphere.obj");
	return model;
}

bool ModelLoader::hasErrorLog()
{
	//检查是否存在错误报告
	return m_log.length()!=0 || textureLoader.hasErrorLog();
}

QString ModelLoader::errorLog()
{
	QString temp = m_log;
	m_log = "";
	return temp;
}

Model * ModelLoader::loadModel(const aiNode * aiNodePtr)
{
	Model* model = new Model;
	model->setObjectName(aiNodePtr->mName.length ? aiNodePtr->mName.C_Str() : "Untitled");
	//依据相应的aiNode,加载对应的mesh以及model
	for (uint32_t i = 0; i < aiNodePtr->mNumMeshes; ++i)
		model->addChildMesh(loadMesh(m_aiScenePtr->mMeshes[aiNodePtr->mMeshes[i]]));
	for (uint32_t i = 0; i < aiNodePtr->mNumChildren; ++i)
		model->addChildModel(loadModel(aiNodePtr->mChildren[i]));

	//计算当前model重心点
	QVector3D center = model->centerOfMass();

	for (int i = 0; i < model->childMeshes().size(); ++i)
		model->childMeshes()[i]->translate(-center);
	for (int i = 0; i < model->childModels().size(); ++i)
		model->childModels()[i]->translate(-center);
	model->translate(center);
	return model;
}

Mesh * ModelLoader::loadMesh(const aiMesh * aiMeshPtr)
{
	Mesh* mesh = new Mesh;
	mesh->setObjectName(aiMeshPtr->mName.length ? aiMeshPtr->mName.C_Str() : "Untitled");

	//遍历当前mesh所具有的顶点数量，并且将其转换为Vertex形式
	for (uint32_t i = 0; i < aiMeshPtr->mNumVertices; ++i)
	{
		Vertex vertex;
		if (aiMeshPtr->HasPositions())
			vertex.position = QVector3D(aiMeshPtr->mVertices[i].x, aiMeshPtr->mVertices[i].y, aiMeshPtr->mVertices[i].z);
		if (aiMeshPtr->HasNormals())
			vertex.normal = QVector3D(aiMeshPtr->mNormals[i].x, aiMeshPtr->mNormals[i].y, aiMeshPtr->mNormals[i].z);
		if (aiMeshPtr->HasTangentsAndBitangents())
		{
			vertex.bitangent = QVector3D(aiMeshPtr->mBitangents[i].x, aiMeshPtr->mBitangents[i].y, aiMeshPtr->mBitangents[i].z);
			vertex.tangent = QVector3D(aiMeshPtr->mTangents[i].x, aiMeshPtr->mTangents[i].y, aiMeshPtr->mTangents[i].z);
			
			//当在更大的网格上计算切线向量的时候，它们往往有很大数量
			//的共享顶点，当发下贴图应用到这些表面时将切线向量平均化
			//通常能获得更好更平滑的结果。这样做有个问题，就是TBN向
			//量可能会不能互相垂直，这意味着TBN矩阵不再是正交矩阵了。
			// Gram-Schmidt process, re-orthogonalize the TBN vectors
			vertex.tangent -= QVector3D::dotProduct(vertex.tangent, vertex.normal) * vertex.normal;
			vertex.tangent.normalize();

			//处理对称的纹理坐标
			if (QVector3D::dotProduct(QVector3D::crossProduct(vertex.tangent, vertex.normal), vertex.bitangent) < 0.0f)
				vertex.tangent = -vertex.tangent;
		}
		if (aiMeshPtr->HasTextureCoords(0))
			vertex.texCoords = QVector2D(aiMeshPtr->mTextureCoords[0][i].x,
				aiMeshPtr->mTextureCoords[0][i].y);
		mesh->m_vertices.push_back(vertex);
	}

	//将对应的indice置入相应的m_indices中
	for (uint32_t i = 0; i < aiMeshPtr->mNumFaces; ++i)
	{
		for (uint32_t j = 0; j < 3; ++j)
		{
			mesh->m_indices.push_back(aiMeshPtr->mFaces[i].mIndices[j]);
		}
	}

	QVector3D center = mesh->centerOfMass();

	for (int i = 0; i < mesh->m_vertices.size(); ++i)
		mesh->m_vertices[i].position -= center;

	mesh->m_position = center;
	mesh->setMaterial(loadMaterial(m_aiScenePtr->mMaterials[aiMeshPtr->mMaterialIndex]));
	return mesh;
}

Material * ModelLoader::loadMaterial(const aiMaterial * aiMaterialPtr)
{
	Material* material = new Material;
	aiColor4D color;
	float value;
	aiString aiStr;

	//从materialptr中读取对应数据并构造数据结构
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_NAME, aiStr))
		material->setObjectName(aiStr.length ? aiStr.C_Str() : "Untitled");
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_AMBIENT, color))
		material->setAmbient((color.r + color.g + color.b) / 3.0f);
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
		material->setDiffuse((color.r + color.g + color.b) / 3.0f);
		material->setColor(QVector3D(color.r, color.g, color.b) / material->diffuse());
	}
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_COLOR_SPECULAR, color))
		material->setSpecular((color.r + color.g + color.b) / 3.0f);
	if (AI_SUCCESS == aiMaterialPtr->Get(AI_MATKEY_SHININESS, value) && !qFuzzyIsNull(value))
		material->setShininess(value);

	//加载texture
	if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_DIFFUSE, 0, &aiStr))
	{
		QString filePath = m_dir.absolutePath() + '/' + QString(aiStr.C_Str()).replace('\\', '/');
		material->setDiffuseTexture(textureLoader.loadFromFile(Texture::Diffuse, filePath));
	}
	if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_SPECULAR, 0, &aiStr))
	{
		QString filePath = m_dir.absolutePath() + '/' + QString(aiStr.C_Str()).replace('\\', '/');
		material->setSpecularTexture(textureLoader.loadFromFile(Texture::Specular, filePath));
	}
	if (AI_SUCCESS == aiMaterialPtr->GetTexture(aiTextureType_HEIGHT, 0, &aiStr))
	{
		QString filePath = m_dir.absolutePath() + '/' + QString(aiStr.C_Str()).replace('\\', '/');
		material->setBumpTexture(textureLoader.loadFromFile(Texture::Bump, filePath));
	}


	return material;
}
