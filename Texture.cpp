#include "Texture.h"

Texture::Texture(TextureType textureType) : QObject(0)
{
	setObjectName("Untitled Texture");
	m_enabled = true;
	m_textureType = textureType;
}

Texture::Texture(const Texture & texture)
{
	setObjectName(texture.objectName());
	m_enabled = true;
	m_textureType = texture.m_textureType;
	m_image = texture.m_image;
}

Texture::~Texture()
{

}

void Texture::dumpObjectInfo(int level)
{
}

void Texture::dumpObjectTree(int level)
{
}

bool Texture::enabled() const
{
	return m_enabled;
}

Texture::TextureType Texture::textureType() const
{
	return m_textureType;
}

const QImage & Texture::image() const
{
	return m_image;
}

void Texture::setEnabled(bool val)
{
	if (m_enabled != val)
	{
		m_enabled = val;
		enabledChanged(m_enabled);
	}
}

void Texture::setTextureType(TextureType textureType)
{
	if (m_textureType != textureType)
	{
		m_textureType = textureType;
		textureTypeChanged(m_textureType);
	}
}

void Texture::setImage(const QImage & image)
{
	if (m_image != image)
	{
		m_image = image;
		imageChanged(m_image);
	}
}
