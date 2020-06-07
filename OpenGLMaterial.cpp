#include "OpenGLMaterial.h"


struct ShaderMaterialInfo
{
	QVector4D color;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
	int useDiffuseMap;
	int useSepecularMap;
	int useBumpMap;
	int padding;
};

//设定一个静态变量，当前静态变量所有的shader共用。
//每当绑定一个material，需要将绑定的shader的信息传入
//当前static变量
static ShaderMaterialInfo shaderMaterialInfo;

//当前类的material info,仍然绑定于当前类的static uniform buffer中
//便于直接传入shader中
OpenGLUniformBufferObject* OpenGLMaterial::m_materialInfo = 0;

OpenGLMaterial::OpenGLMaterial(Material* material, QObject* parent /* = 0 */)
{
	m_host = material;

	this->diffuseTextureChanged(m_host->diffuseTexture());
	this->specularTextureChanged(m_host->specularTexture());
	this->bumpTextureChanged(m_host->bumpTexture());

	connect(m_host, SIGNAL(diffuseTextureChanged(QSharedPointer<Texture>)), this, SLOT(diffuseTextureChanged(QSharedPointer<Texture>)));
	connect(m_host, SIGNAL(specularTextureChanged(QSharedPointer<Texture>)), this, SLOT(specularTextureChanged(QSharedPointer<Texture>)));
	connect(m_host, SIGNAL(bumpTextureChanged(QSharedPointer<Texture>)), this, SLOT(bumpTextureChanged(QSharedPointer<Texture>)));
	connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));

	setParent(parent);
}

Material * OpenGLMaterial::host() const
{
	return m_host;
}

void OpenGLMaterial::bind()
{
	shaderMaterialInfo.useDiffuseMap = false;
	shaderMaterialInfo.useSepecularMap = false;
	shaderMaterialInfo.useBumpMap = false;

	if (m_openGLDiffuseTexture)
		shaderMaterialInfo.useDiffuseMap = m_openGLDiffuseTexture->bind();
	if (m_openGLSpecularTexture)
		shaderMaterialInfo.useSepecularMap = m_openGLSpecularTexture->bind();
	if (m_openGLBumpTexture)
		shaderMaterialInfo.useBumpMap = m_openGLBumpTexture->bind();

	shaderMaterialInfo.color = m_host->color();
	shaderMaterialInfo.ambient = m_host->ambient();
	shaderMaterialInfo.diffuse = m_host->diffuse();
	shaderMaterialInfo.specular = m_host->specular();
	shaderMaterialInfo.shininess = m_host->shininess();

	//若当前material info uniformbuffer没有初始化，那么就进行初始化
	if (m_materialInfo == 0)
	{
		m_materialInfo = new OpenGLUniformBufferObject;
		m_materialInfo->create();
		m_materialInfo->bind();
		m_materialInfo->allocate(MATERIAL_INFO_BINDING_POINT, NULL, sizeof(ShaderMaterialInfo));
		m_materialInfo->release();
	}
	m_materialInfo->bind();
	m_materialInfo->write(0, &shaderMaterialInfo, sizeof(ShaderMaterialInfo));
	m_materialInfo->release();
}

void OpenGLMaterial::release()
{
	if (m_openGLDiffuseTexture)
		m_openGLDiffuseTexture->release();
	if (m_openGLSpecularTexture)
		m_openGLSpecularTexture->release();
	if (m_openGLBumpTexture)
		m_openGLBumpTexture->release();

	shaderMaterialInfo.color = QVector3D(0, 0, 0);
	shaderMaterialInfo.useDiffuseMap = 0;
	shaderMaterialInfo.useSepecularMap = 0;
	shaderMaterialInfo.useBumpMap = 0;

	if (m_materialInfo == 0)
	{
		m_materialInfo = new OpenGLUniformBufferObject;
		m_materialInfo->create();
		m_materialInfo->bind();
		m_materialInfo->allocate(3, NULL, sizeof(ShaderMaterialInfo));
		m_materialInfo->release();
	}
	m_materialInfo->bind();
	m_materialInfo->write(0, &shaderMaterialInfo, sizeof(ShaderMaterialInfo));
	m_materialInfo->release();
}


void OpenGLMaterial::diffuseTextureChanged(QSharedPointer<Texture> diffuseTexture)
{
	if (diffuseTexture.isNull())//如果diffuseTexture为空格，那么opengl端也为空
	{
		m_openGLDiffuseTexture = 0;
	}
	else if (diffuseTexture->property("OpenGLTexturePointer").isValid())
	{
		//如果openGLTexture的属性为有效的，那么用属性赋值
		m_openGLDiffuseTexture = diffuseTexture->property("OpenGLTexturePointer").value<OpenGLTexture*>();
	}
	else
	{
		//如果当前Texture什么都没有，通过diffuseTexture来初始化openGL端
		m_openGLDiffuseTexture = new OpenGLTexture(diffuseTexture.data());
	}
}

void OpenGLMaterial::specularTextureChanged(QSharedPointer<Texture> specularTexture)
{
	if (specularTexture.isNull())
	{
		m_openGLSpecularTexture = 0;
	}
	else if (specularTexture->property("OpenGLTexturePointer").isValid())
	{
		m_openGLSpecularTexture = specularTexture->property("OpenGLTexturePointer").value<OpenGLTexture*>();
	}
	else
	{
		m_openGLSpecularTexture = new OpenGLTexture(specularTexture.data());
	}
}

void OpenGLMaterial::bumpTextureChanged(QSharedPointer<Texture> bumpTexture)
{
	if (bumpTexture.isNull())
	{
		m_openGLBumpTexture = 0;
	}
	else if (bumpTexture->property("OpenGLTexturePointer").isValid())
	{
		m_openGLBumpTexture = bumpTexture->property("OpenGLTexturePointer").value<OpenGLTexture*>();
	}
	else
	{
		m_openGLBumpTexture = new OpenGLTexture(bumpTexture.data());
	}
}

void OpenGLMaterial::hostDestroyed(QObject * host)
{
	delete this;
}
