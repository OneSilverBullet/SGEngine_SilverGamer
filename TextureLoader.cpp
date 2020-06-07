#include "TextureLoader.h"

QHash<QString, QWeakPointer<Texture>> TextureLoader::cache;

QSharedPointer<Texture> TextureLoader::loadFromFile(Texture::TextureType textureType, QString filePath)
{
	if (cache[filePath].isNull())
	{
		//����ͼ��QImage
		QSharedPointer<Texture> texture(new Texture(textureType));
		QImageReader reader(filePath);
		texture->setObjectName(filePath);
		texture->setImage(reader.read());
		if (texture->image().isNull())
		{
			//����ʧ��
			return QSharedPointer<Texture>();
		}
		//���ص�cache
		cache[filePath] = texture;
		return texture;
	}
	//ֱ�Ӵ�cache���н��м���
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
