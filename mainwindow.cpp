#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	resize(1200, 720);

	configMenu();
	configLayout();
	configSignals();

	

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configMenu()
{
	QMenu* menuFile = menuBar()->addMenu("File");
	menuFile->addAction("New Scene", this, SLOT(), QKeySequence(Qt::CTRL + Qt::Key_N));
	menuFile->addAction("Open Scene", this, SLOT(), QKeySequence(Qt::CTRL + Qt::Key_O));
	menuFile->addSeparator();
	menuFile->addAction("Import Model", this, SLOT());
	menuFile->addAction("Export Model", this, SLOT());
	menuFile->addSeparator();
	menuFile->addAction("Save Scene", this, SLOT());
	menuFile->addAction("Save Scene As", this, SLOT(), QKeySequence(Qt::CTRL + Qt::Key_S));
	menuFile->addSeparator();
	menuFile->addAction("Quit", this, SLOT(), QKeySequence(Qt::Key_Backspace));


	QMenu* menuEdit = menuBar()->addMenu("Edit");
	menuEdit->addAction("Copy", this, SLOT(), QKeySequence(Qt::CTRL + Qt::Key_C));
	menuEdit->addAction("Paste", this, SLOT(), QKeySequence(Qt::CTRL + Qt::Key_V));
	menuEdit->addSeparator();
	menuEdit->addAction("Remove", this, SLOT(), QKeySequence(Qt::Key_Backspace));

	QMenu* menuCreate = menuBar()->addMenu("Create");
	menuCreate->addAction("GridLine", this, SLOT());

	QMenu* menuCreateLight = menuCreate->addMenu("Light");
	menuCreateLight->addAction("Ambient Light", this, SLOT());

	QMenu* menuGizmo = menuBar()->addMenu("Gizmo");
	QAction* actionGizmoAlwaysOnTop = menuGizmo->addAction("Always on Top", this, SLOT());
	actionGizmoAlwaysOnTop->setCheckable(true);
	menuGizmo->addSeparator();
	QAction* actionGizmoTypeTranslate = menuGizmo->addAction("Translate", this, SLOT());
	actionGizmoTypeTranslate->setCheckable(true);
	QAction* actionGizmoTypeRotation = menuGizmo->addAction("Rotation", this, SLOT());
	actionGizmoTypeRotation->setCheckable(true);
	QAction* actionGizmoTypeScale = menuGizmo->addAction("Scale", this, SLOT());
	actionGizmoTypeScale ->setCheckable(true);

	QActionGroup * actionAxisTypeGroup = new QActionGroup(menuGizmo);
	actionAxisTypeGroup->addAction(actionGizmoTypeTranslate);
	actionAxisTypeGroup->addAction(actionGizmoTypeRotation);
	actionAxisTypeGroup->addAction(actionGizmoTypeScale);
	
	actionGizmoAlwaysOnTop->setChecked(true);
	actionGizmoTypeTranslate->setChecked(true);


	QMenu* menuHelp = menuBar()->addMenu("Help");
	menuHelp->addAction("Check Update", this, SLOT());


}

void MainWindow::configLayout()
{
	m_splitter = new QSplitter(this);
	//m_splitter->addWidget(m_sceneTreeWidget);
	m_splitter->setSizes(QList<int>{160, width() - 160 - 300, 300});


	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addWidget(m_splitter);
	centralWidget()->setLayout(mainLayout);

}

void MainWindow::configSignals()
{
}
