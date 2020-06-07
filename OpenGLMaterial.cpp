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

//�趨һ����̬��������ǰ��̬�������е�shader���á�
//ÿ����һ��material����Ҫ���󶨵�shader����Ϣ����
//��ǰstatic����
static ShaderMaterialInfo shaderMaterialInfo;

//��ǰ���material info,��Ȼ���ڵ�ǰ���static uniform buffer��
//����ֱ�Ӵ���shader��
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

	//����ǰmaterial info uniformbufferû�г�ʼ������ô�ͽ��г�ʼ��
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
	if (diffuseTexture.isNull())//���diffuseTextureΪ�ո���ôopengl��ҲΪ��
	{
		m_openGLDiffuseTexture = 0;
	}
	else if (diffuseTexture->property("OpenGLTexturePointer").isValid())
	{
		//���openGLTexture������Ϊ��Ч�ģ���ô�����Ը�ֵ
		m_openGLDiffuseTexture = diffuseTexture->property("OpenGLTexturePointer").value<OpenGLTexture*>();
	}
	else
	{
		//�����ǰTextureʲô��û�У�ͨ��diffuseTexture����ʼ��openGL��
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
