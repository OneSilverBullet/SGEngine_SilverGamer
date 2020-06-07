#pragma once


#include <Model.h>
#include <Vector3DEditSlider.h>

class ModelProperty : public QWidget
{
	Q_OBJECT

public:
	ModelProperty(Model* model, QWidget*  parent = 0);

private:
	Model* m_host;
	QCheckBox* m_visibleCheckBox, *m_wireFrameModeCheckBox;
	QLabel* m_numOfChildMeshTextLabel, *m_numOfChildMeshValueLabel;
	QLabel* m_numOfChildModelTextLabel, *m_numOfChildModelValueLabel;
	Vector3DEdit* m_positionEdit, *m_scalingEdit;
	Vector3DEditSlider* m_rotationEditSlider;

	void configLayout();
	void configSignals();

public slots:
	void hostDestroyed(QObject* host);
	void childMeshAdded(Mesh* mesh);
	void childMeshRemoved(QObject* object);
	void childModelAdded(Model* model);
	void childModelRemoved(QObject* object);
};