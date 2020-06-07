#pragma once

#include <GridLine.h>
#include <Vector3DEditSlider.h>


class GridLineProperty : public QWidget {
	Q_OBJECT

public:
	GridLineProperty(GridLine* gridline, QWidget* parent = 0);

private:
	GridLine* m_host;
	Vector3DEdit* m_xargsEdit, *m_yargsEdit, *m_zargsEdit;
	Vector3DEditSlider* m_colorEditSlider;

	void configLayout();
	void configSignals();

private slots:
	void hostDestroyed(QObject*);


};