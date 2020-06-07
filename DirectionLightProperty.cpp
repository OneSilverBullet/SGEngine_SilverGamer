#include "DirectionLightProperty.h"

DirectionLightProperty::DirectionLightProperty(DirectionLight* light, QWidget* parent /* = 0 */)
{
	m_host = light;

	m_enabledCheckbox = new QCheckBox("Enabled", this);
	m_intensityLabel = new QLabel("Intensity", this);
	m_intensityEdit = new FloatEdit(0.0f, inf, 2, this);
	m_intensitySlider = new FloatSlider(Qt::Horizontal, 0.0f, 1.0f, this);
	m_colorEditSlider = new Vector3DEditSlider("Color", Qt::Horizontal, "R", "G", "B", 0.0f, 1.0f, 2, this);
	m_directionEdit = new Vector3DEdit("Direction", Qt::Horizontal, "X", "Y", "Z", -inf, inf, 2, this);

	m_enabledCheckbox->setChecked(m_host->enabled());
	m_intensityEdit->setValue(m_host->intensity());
	m_intensitySlider->setValue(m_host->intensity());
	m_colorEditSlider->setValue(m_host->color());
	m_directionEdit->setValue(m_host->direction());

	configLayout();
	configSignals();
}

void DirectionLightProperty::configLayout()
{
	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->setAlignment(Qt::AlignTop);
	mainLayout->addWidget(m_enabledCheckbox, 0, 0);
	mainLayout->addWidget(m_intensityLabel, 1, 0);
	mainLayout->addWidget(m_intensityEdit, 1, 1);
	mainLayout->addWidget(m_intensitySlider, 1, 2);
	mainLayout->addWidget(m_colorEditSlider, 2, 0, 1, 3);
	mainLayout->addWidget(m_directionEdit, 3, 0, 1, 3);
	setLayout(mainLayout);
}

void DirectionLightProperty::configSignals()
{
	connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));

	//check box控制小组件的激活状态
	connect(m_enabledCheckbox, SIGNAL(toggled(bool)), m_host, SLOT(setEnabled(bool)));
	connect(m_enabledCheckbox, SIGNAL(toggled(bool)), m_intensityEdit, SLOT(setEnabled(bool)));
	connect(m_enabledCheckbox, SIGNAL(toggled(bool)), m_intensitySlider, SLOT(setEnabled(bool)));
	connect(m_enabledCheckbox, SIGNAL(toggled(bool)), m_colorEditSlider, SLOT(setEnabled(bool)));
	connect(m_enabledCheckbox, SIGNAL(toggled(bool)), m_directionEdit, SLOT(setEnabled(bool)));

	//小组件的状态影响host的状态
	connect(m_intensityEdit, SIGNAL(valueEdited(float)), m_host, SLOT(setIntensity(float)));
	connect(m_intensitySlider, SIGNAL(valueEdited(float)), m_host, SLOT(setIntensity(float)));
	connect(m_colorEditSlider, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setColor(QVector3D)));
	connect(m_directionEdit, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setDirection(QVector3D)));

	//host的状态影响小组件的状态
	connect(m_host, SIGNAL(enabledChanged(bool)), m_enabledCheckbox, SLOT(setChecked(bool)));
	connect(m_host, SIGNAL(intensityChanged(float)), m_intensityEdit, SLOT(setValue(float)));
	connect(m_host, SIGNAL(intensityChanged(float)), m_intensitySlider, SLOT(setValue(float)));
	connect(m_host, SIGNAL(colorChanged(QVector3D)), m_colorEditSlider, SLOT(setValue(QVector3D)));
	connect(m_host, SIGNAL(directionChanged(QVector3D)), m_directionEdit, SLOT(setValue(QVector3D)));


}

void DirectionLightProperty::hostDestroyed(QObject* host)
{
	delete this;
}