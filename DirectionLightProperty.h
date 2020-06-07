#pragma once

#include <DirectionLight.h>
#include <Vector3DEditSlider.h>

class DirectionLightProperty :public QWidget
{
	Q_OBJECT

public:
	DirectionLightProperty(DirectionLight* light, QWidget* parent = 0);


private:
	DirectionLight* m_host;
	QCheckBox* m_enabledCheckbox;
	QLabel* m_intensityLabel;
	FloatEdit* m_intensityEdit;
	FloatSlider* m_intensitySlider;
	Vector3DEditSlider* m_colorEditSlider;
	Vector3DEdit* m_directionEdit;

	void configLayout();
	void configSignals();

private slots:
	void hostDestroyed(QObject* host);


};
