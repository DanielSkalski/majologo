#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "Panel.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QPainter>
#include <QLineF>
#include <QTextEdit>
#include <QStatusBar>
#include "Interpreter.h"

class mainWidget : public QWidget
{
    Q_OBJECT
protected:
friend class MainWindow;

    Panel *panel;
    QLineEdit *cmdLine;
    QPushButton *butt;
    QPushButton *sendButt;
    Interpreter *interpreter;
    QTextEdit *historyView;
    QStatusBar *status;

private slots:
    void komenda();
    void komenda(QString);

public:
    mainWidget(QWidget *parent = 0);
    void makeTheLook();
};

#endif // MAINWIDGET_H
