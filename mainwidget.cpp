#include "mainwidget.h"
#include <QMessageBox>


mainWidget::mainWidget(QWidget *parent) : QWidget(parent)
{
    panel = new Panel;
    interpreter = new Interpreter(panel);
    cmdLine = new QLineEdit;
    butt = new QPushButton("Quit");
    sendButt = new QPushButton("Apply");
    historyView = new QTextEdit;
    historyView->setReadOnly(true);
    status = new QStatusBar;
    status->showMessage("Turtle got moves. Witaj w majoLogo :)");


    makeTheLook();

    interpreter->panelWithTurtle();
    panel->update();

    connect(sendButt, SIGNAL(clicked()), this, SLOT(komenda()));
    connect(cmdLine, SIGNAL(returnPressed()), this, SLOT(komenda()));
}

void mainWidget::makeTheLook()
{
    QGridLayout* mainLayout = new QGridLayout(this);

    mainLayout->addWidget(historyView, 0, 3);
    mainLayout->addWidget(panel, 0, 0, 0, 3);
    mainLayout->addWidget(cmdLine, 1, 0);
    mainLayout->addWidget(sendButt, 1, 2);
    mainLayout->setColumnStretch(0, 2);
    mainLayout->addWidget(butt, 1, 3);
    mainLayout->addWidget(status, 2, 0);

    this->setLayout(mainLayout);
}

void mainWidget::komenda()
{
    if(!cmdLine->text().isEmpty()) // sprawdzenie czy nie puste?
    {
        try{
            interpreter->getCommand( cmdLine->text() );
            historyView->append( cmdLine->text().toUpper() );
            historyView->update();
            interpreter->panelWithTurtle();
            panel->update();
            cmdLine->clear();
            status->showMessage("Polecenie wykonane!");
        }catch(QString msg)
        {
            QMessageBox msgBox;
            msgBox.setText(msg);
            msgBox.exec();
            status->showMessage("Zolw nie rozumie o co Ci chodzi...");
        }
    }
}

void mainWidget::komenda(QString str)
{
    if(!str.isEmpty())
    {
        try{
            interpreter->getCommand( str );
            historyView->append( str );
            historyView->update();
            interpreter->panelWithTurtle();
            panel->update();
        }
        catch(QString)
        {
            QMessageBox msgBox;
            msgBox.setText("Plik zawiera nieprawidlowe dane.");
            msgBox.exec();
        }
    }
    return;
}
