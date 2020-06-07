#include "ModelProperty.h"

ModelProperty::ModelProperty(Model* model, QWidget* parent /* = 0 */)
{
	m_host = model;

	m_visibleCheckBox = new QCheckBox("Visible", this);
	m_wireFrameModeCheckBox = new QCheckBox("WireFrame Mode", this);
	m_numOfChildMeshTextLabel = new QLabel("Child Meshes:", this);
	m_numOfChildMeshValueLabel = new QLabel(QString::number(m_host->childMeshes().size()), this);
	m_numOfChildModelTextLabel = new QLabel("Child Models:", this);
	m_numOfChildModelValueLabel = new QLabel(QString::number(m_host->childModels().size()), this);

	m_positionEdit = new Vector3DEdit("Position", Qt::Horizontal, "X", "Y", "Z", -inf, inf, 2, this);
	m_rotationEditSlider = new Vector3DEditSlider("Rotation", Qt::Horizontal, "X", "Y", "Z", -180.0f, 180.0f, 2, this);
	m_scalingEdit = new Vector3DEdit("Scaling", Qt::Horizontal, "X", "Y", "Z", 0.0f, inf, 2, this);

	m_visibleCheckBox->setChecked(m_host->visible());
	m_wireFrameModeCheckBox->setChecked(m_host->wireFrameMode());
	m_positionEdit->setValue(m_host->position());
	m_rotationEditSlider->setValue(m_host->rotation());
	m_scalingEdit->setValue(m_host->scaling());

	configLayout();
	configSignals();
}

void ModelProperty::configLayout()
{
	QGridLayout* subLayout = new QGridLayout;
	subLayout->setAlignment(Qt::AlignTop);
	subLayout->setVerticalSpacing(10);
	subLayout->addWidget(m_visibleCheckBox, 0, 0, 1, 2);
	subLayout->addWidget(m_wireFrameModeCheckBox, 1, 0, 1, 2);
	subLayout->addWidget(m_numOfChildMeshTextLabel, 2, 0);
	subLayout->addWidget(m_numOfChildMeshValueLabel, 2, 1);
	subLayout->addWidget(m_numOfChildModelTextLabel, 3, 0);
	subLayout->addWidget(m_numOfChildModelValueLabel, 3, 1);
	subLayout->addWidget(m_positionEdit, 4, 0, 1, 2);
	subLayout->addWidget(m_rotationEditSlider, 5, 0, 1, 2);
	subLayout->addWidget(m_scalingEdit, 6, 0, 1, 2);

	setLayout(subLayout);
}

void ModelProperty::configSignals()
{
	//控制对应的相应的model与property之间的关系，及时更新模型的情况
	connect(m_host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
	connect(m_host, SIGNAL(childMeshAdded(Mesh*)), this, SLOT(childMeshAdded(Mesh*)));
	connect(m_host, SIGNAL(childMeshRemoved(QObject*)), this, SLOT(childMeshRemoved(QObject*)));
	connect(m_host, SIGNAL(childModelAdded(Model*)), this, SLOT(childModelAdded(Model*)));
	connect(m_host, SIGNAL(childModelRemoved(QObject*)), this, SLOT(childModelRemoved(QObject*)));

	//property中可见性模块与其他模块之间的关系设置
	connect(m_visibleCheckBox, SIGNAL(toggled(bool)), m_wireFrameModeCheckBox, SLOT(setEnabled(bool)));
	connect(m_visibleCheckBox, SIGNAL(toggled(bool)), m_positionEdit, SLOT(setEnabled(bool)));
	connect(m_visibleCheckBox, SIGNAL(toggled(bool)), m_rotationEditSlider, SLOT(setEnabled(bool)));
	connect(m_visibleCheckBox, SIGNAL(toggled(bool)), m_scalingEdit , SLOT(setEnabled(bool)));

	//property中子模块与host之间的关系设置
	connect(m_visibleCheckBox, SIGNAL(toggled(bool)), m_host, SLOT(setVisible(bool)));
	connect(m_wireFrameModeCheckBox, SIGNAL(toggled(bool)), m_host, SLOT(setWireFrameMode(bool)));
	connect(m_positionEdit, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setPosition(QVector3D)));
	connect(m_rotationEditSlider, SIGNAL(valueEdited(QVector3D)), m_host, SLOT(setRotation(QVector3D)));
	connect(m_scalingEdit, SIGNAL(valueEdited(QVector3D)), this, SLOT(setScaling(QVector3D)));

	//设置host与prperty之间的关系
	connect(m_host, SIGNAL(visibleChanged(bool)), m_visibleCheckBox, SLOT(setChecked(bool)));
	connect(m_host, SIGNAL(wireFrameModeChanged(bool)), m_wireFrameModeCheckBox, SLOT(setChecked(bool)));
	connect(m_host, SIGNAL(positionChanged(QVector3D)), m_positionEdit, SLOT(setValue(QVector3D)));
	connect(m_host, SIGNAL(rotationChanged(QVector3D)), m_rotationEditSlider, SLOT(setValue(QVector3D)));
	connect(m_host, SIGNAL(scalingChanged(QVector3D)), m_scalingEdit, SLOT(setValue(QVector3D)));
}


void ModelProperty::hostDestroyed(QObject* host)
{
	delete this;
}

void ModelProperty::childMeshAdded(Mesh* mesh)
{
	m_numOfChildMeshValueLabel->setText(QString::number(m_host->childMeshes().size()));
}

void ModelProperty::childMeshRemoved(QObject* object)
{
	m_numOfChildMeshValueLabel->setText(QString::number(m_host->childMeshes().size()));
}

void ModelProperty::childModelAdded(Model* model)
{
	m_numOfChildModelValueLabel->setText(QString::number(m_host->childModels().size()));
}

void ModelProperty::childModelRemoved(QObject* object)
{
	m_numOfChildModelValueLabel->setText(QString::number(m_host->childModels().size()));
}
