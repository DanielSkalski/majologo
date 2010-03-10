#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QImageWriter>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QColorDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    main = new mainWidget;
    setCentralWidget(main);

    connect(main->butt, SIGNAL(clicked()), this, SLOT(close()));

    createActions();
    createMenus();

    setWindowTitle("majoLOGO");
    setMinimumSize(900, 690);
    resize(960, 700);
}

MainWindow::~MainWindow()
{}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&Nowy"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Stwórz nowy LOGOrysunek"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Otwórz"), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Otwórz plik ze zbiorem komend LOGO"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    aboutAct = new QAction(tr("O &programie"), this);
    aboutAct->setShortcut(tr("Ctrl+P"));
    aboutAct->setStatusTip(tr("Nieco info o majoLOGO"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    exitAct = new QAction("WyjdŸ", this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    saveToImgAct = new QAction("Zapisz obrazek", this);
    connect(saveToImgAct, SIGNAL(triggered()), this, SLOT(saveToImg()));

    saveCmdsAct = new QAction("Zapisz do pliku", this);
    connect(saveCmdsAct, SIGNAL(triggered()), this, SLOT(saveCmds()));

    // akcje z menu tools
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(aboutAct);
    fileMenu->addAction(saveToImgAct);
    fileMenu->addAction(saveCmdsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    toolMenu = menuBar()->addMenu("Tools");
}


 void MainWindow::newFile()
 {
    delete main;
    main = new mainWidget;
    setCentralWidget(main);
    connect(main->butt, SIGNAL(clicked()), this, SLOT(close()));
 }

 void MainWindow::open()
 {
    newFile();
    QString name = QFileDialog::getOpenFileName(this, "Otworz plik", "", "pliki majoLOGO(*.mlogo)");
    QFile file(name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString line;
    do
    {
        line = in.readLine();
        main->komenda( line );
    }
    while( !line.isNull() );
 }

void MainWindow::about()
 {
     QMessageBox::about(this, "Wielkie O",
             "Ale osohozzi?");
 }

void MainWindow::saveToImg()
{
    QString name = QFileDialog::getSaveFileName(this, "Zapisz obrazek", "watchMe.png", "plik PNG (*.png)");
    QImageWriter writer(name);
    writer.write(main->interpreter->intrPanel->pixmap->toImage());
}

void MainWindow::saveCmds()
{
    QString name = QFileDialog::getSaveFileName(this, "Zapisz polecenia", "turtleGotMoves.mlogo", "plik majoLogo(*.mlogo)");
    QFile file(name);
    file.open(QFile::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << main->historyView->toPlainText();
    file.close();
}

