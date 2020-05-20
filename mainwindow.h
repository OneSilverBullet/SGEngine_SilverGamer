#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SceneTreeWidge.h>
#include <Common.h>




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

	QSplitter* m_splitter;

	SceneTreeWidge* m_sceneTreeWidget;

	void configMenu();
	void configLayout();
	void configSignals();


};

#endif // MAINWINDOW_H
