#include "OpenGLTexture.h"

#include <Common.h>

OpenGLTexture::OpenGLTexture(Texture* texture)
{
	m_host = texture;
	m_openglTexture = 0;

	//由于当前类继承于QObject，因此可以直接转换为QVariant。否则需要进行注册Meta_Type
	//这是将Draw与Logic建立联系的方法：Property
	m_host->setProperty("OpenGL Texture Pointer", QVariant::fromValue(this));

	connect(m_host, SIGNAL(imageChanged(QImage)), this, SLOT(imageChanged(QImage)));
	connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
}

OpenGLTexture::~OpenGLTexture()
{
	delete m_openglTexture;
	m_host->setProperty("OpenGL Texture Pointer", QVariant());
}

void OpenGLTexture::create()
{
	//从设备坐标系到纹理坐标系，用mirrored进行转换一下
	m_openglTexture = new QOpenGLTexture(m_host->image().mirrored());
	//设置与纹理相关的内容
	m_openglTexture->setMinificationFilter(QOpenGLTexture::Nearest);
	m_openglTexture->setMagnificationFilter(QOpenGLTexture::Linear);
	m_openglTexture->setWrapMode(QOpenGLTexture::Repeat);
}

//将当前OpenGLTexture的iD绑定到对应的位置
bool OpenGLTexture::bind()
{
	if (!m_openglTexture) create();
	if (!m_host->enabled()) return false;

	//QOpenGLFunctions* glFuncs = QOpenGLContext::currentContext()->functions();
	QOpenGLFunctions_3_3_Core* glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	if (m_host->textureType() == Texture::Diffuse)
	{
		glFuncs->glActiveTexture(GL_TEXTURE0 + 0);
		glFuncs->glBindTexture(GL_TEXTURE_2D, m_openglTexture->textureId());
	}
	else if (m_host->textureType() == Texture::Specular)
	{
		glFuncs->glActiveTexture(GL_TEXTURE0 + 1);
		glFuncs->glBindTexture(GL_TEXTURE_2D, m_openglTexture->textureId());
	}
	else if (m_host->textureType() == Texture::Bump)
	{
		glFuncs->glActiveTexture(GL_TEXTURE0 + 2);
		glFuncs->glBindTexture(GL_TEXTURE_2D, m_openglTexture->textureId());
	}
	return true;
}

void OpenGLTexture::release()
{
	QOpenGLFunctions_3_3_Core* glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	if (m_host->textureType() == Texture::Diffuse)
	{
		glFuncs->glActiveTexture(GL_TEXTURE0 + 0);
		glFuncs->glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (m_host->textureType() == Texture::Specular)
	{
		glFuncs->glActiveTexture(GL_TEXTURE0 + 1);
		glFuncs->glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (m_host->textureType() == Texture::Bump)
	{
		glFuncs->glActiveTexture(GL_TEXTURE0 + 2);
		glFuncs->glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void OpenGLTexture::imageChanged(const QImage& image)
{
	if (m_openglTexture)
	{
		delete m_openglTexture;
		m_openglTexture = new QOpenGLTexture(image);
		m_openglTexture->setMinificationFilter(QOpenGLTexture::Nearest);
		m_openglTexture->setMagnificationFilter(QOpenGLTexture::Linear);
		m_openglTexture->setWrapMode(QOpenGLTexture::Repeat);
	}
}

void OpenGLTexture::hostDestroyed(QObject * host)
{
	delete this;
}
