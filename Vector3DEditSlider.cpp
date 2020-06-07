#include "Vector3DEditSlider.h"

Vector3DEditSlider::Vector3DEditSlider(QWidget* parent) :Vector3DEdit(parent)
{
	for (int i = 0; i < 3; ++i)
	{
		m_floatSlider[i] = new FloatSlider(this);
	}
	configLayout();
	configSignals();
}


Vector3DEditSlider::Vector3DEditSlider(QString title, Qt::Orientation orientation, QString label0, QString label1, QString label2, float minimum, float maximum, int decimals, QWidget * parent)
	:Vector3DEdit(title, orientation, label0, label1, label2, minimum, maximum, decimals, parent)
{
	for (int i = 0; i < 3; ++i)
	{
		m_floatSlider[i] = new FloatSlider(Qt::Horizontal, minimum, maximum, this);
	}
	configLayout(orientation);
	configSignals();
}

void Vector3DEditSlider::configLayout(Qt::Orientation orientation)
{
	QGridLayout* mainLayout = static_cast<QGridLayout*>(this->layout());
	for (int i = 0; i < 3; ++i)
	{
		if (orientation = Qt::Horizontal)
		{
			mainLayout->addWidget(m_floatSlider[i], 2, i);
		}
		else
		{
			mainLayout->addWidget(m_floatSlider[i], i, 2);
		}
	}
}

void Vector3DEditSlider::configSignals()
{
	for (int i = 0; i < 3; ++i)
	{
		connect(m_floatEdits[i], SIGNAL(valueChanged(float)), this, SLOT(setValue(float)));
		connect(m_floatSlider[i], SIGNAL(valueChanged(float)), this, SLOT(setValue(float)));
		connect(m_floatSlider[i], SIGNAL(sliderMoved(float)), this, SLOT(floatSliderValueSlided(float)));
	}
}
