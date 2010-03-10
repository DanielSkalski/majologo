#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QAction>
#include <QMenuBar>

#include "mainWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    mainWidget* main;
private slots:
    void newFile();
    void open();
    void saveToImg();
    void saveCmds();
    void about();
private:
    void createActions();
    void createMenus();
    QMenu * fileMenu;
    QMenu * toolMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveToImgAct;
    QAction *saveCmdsAct;
    QAction *aboutAct;
    QAction *exitAct;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
