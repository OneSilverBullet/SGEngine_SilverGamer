#include "TextureLoader.h"

QHash<QString, QWeakPointer<Texture>> TextureLoader::cache;

QSharedPointer<Texture> TextureLoader::loadFromFile(Texture::TextureType textureType, QString filePath)
{
	if (cache[filePath].isNull())
	{
		//加载图像QImage
		QSharedPointer<Texture> texture(new Texture(textureType));
		QImageReader reader(filePath);
		texture->setObjectName(filePath);
		texture->setImage(reader.read());
		if (texture->image().isNull())
		{
			//加载失败
			return QSharedPointer<Texture>();
		}
		//加载到cache
		cache[filePath] = texture;
		return texture;
	}
	//直接从cache当中进行加载
	return cache[filePath];
}

bool TextureLoader::hasErrorLog()
{
	return m_log != "";
}

QString TextureLoader::errorLog()
{
	QString tmp = m_log;
	m_log = "";
	return tmp;
}
