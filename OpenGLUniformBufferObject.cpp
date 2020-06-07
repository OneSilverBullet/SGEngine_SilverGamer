#include "OpenGLUniformBufferObject.h"

QOpenGLFunctions_3_3_Core* OpenGLUniformBufferObject::glFuncs = 0;

OpenGLUniformBufferObject::OpenGLUniformBufferObject()
{
	m_id = 0;
	glFuncs = 0;
}

OpenGLUniformBufferObject::~OpenGLUniformBufferObject()
{
	if (m_id)
	{
		destroy();
	}
}

GLuint OpenGLUniformBufferObject::bufferID() const
{
	return m_id;
}

//初始化glFuncs同时进行Buffer构造
bool OpenGLUniformBufferObject::create()
{
	if (m_id) destroy();
	if (!glFuncs)
	{
		glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	}
	glFuncs->glGenBuffers(1, &m_id);
	return m_id;
}

void OpenGLUniformBufferObject::allocate(int indx, const void * data, int count)
{
	if (m_id == 0)return;
	if (!glFuncs) return;
	glFuncs->glBufferData(GL_UNIFORM_BUFFER, count, data, GL_STATIC_DRAW);
	glFuncs->glBindBufferRange(GL_UNIFORM_BUFFER, indx, m_id, 0, count);
}

void OpenGLUniformBufferObject::destroy()
{
	//如果当前两者都分配了，那么就释放当前的buffer
	if (m_id && glFuncs)
	{
		glFuncs->glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
}

//绑定当前uniformbuffer object
void OpenGLUniformBufferObject::bind()
{
	if (m_id && glFuncs)
		glFuncs->glBindBuffer(GL_UNIFORM_BUFFER, m_id);
}

//已经存在uniformbuffer的情况下向buffer当中写入
void OpenGLUniformBufferObject::write(int offset, const void * data, int count)
{
	if (m_id && glFuncs)
		glFuncs->glBufferSubData(GL_UNIFORM_BUFFER, offset, count, data);
}

void OpenGLUniformBufferObject::release()
{
	if (m_id && glFuncs)
		glFuncs->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

//用于得到当前shader所有的对应的Uniform block indx
//然后将对应的idx绑定到对应的pointing上
void OpenGLUniformBufferObject::bindUniformBlock(QOpenGLShaderProgram * shader)
{
	if (!glFuncs)
		glFuncs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	GLuint indx = glFuncs->glGetUniformBlockIndex(shader->programId(), "CameraInfo");
	glFuncs->glUniformBlockBinding(shader->programId(), indx, CAMERA_INFO_BINDING_POINT);
	indx = glFuncs->glGetUniformBlockIndex(shader->programId(), "ModelInfo");
	glFuncs->glUniformBlockBinding(shader->programId(), indx, MODEL_INFO_BINDING_POINT);
	indx = glFuncs->glGetUniformBlockIndex(shader->programId(), "MaterialInfo");
	glFuncs->glUniformBlockBinding(shader->programId(), indx, MATERIAL_INFO_BINDING_POINT);
	indx = glFuncs->glGetUniformBlockIndex(shader->programId(), "LightInfo");
	glFuncs->glUniformBlockBinding(shader->programId(), indx, LIGHT_INFO_BINDING_POINT);

}
