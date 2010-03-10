#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QWidget>
#include <QString>
#include <QPointF>
#include <QColor>
#include <QLineF>
#include <QMap>
#include "Panel.h"

class Interpreter : public QWidget
{
    Q_OBJECT
    friend class mainWidget;
    friend class MainWindow;
    class Turtle
    {
    friend class Interpreter;
        QPointF *position;
        qreal angle;
        bool down;
    public:
        QColor color;
        QLineF img[3];
        QPixmap turtl;
        QPen pen;
        Turtle(QPointF *p)
        {
            color = QColor(0, 0, 0);
            position = p;
            angle = 0.0;
            down = true;
            setImg();
        }
        ~Turtle()
        {
            delete position;
        }
        void setAngle(qreal ang)
        {
            if(ang < 0)
                ang = 360 + ang;
            ang = (int)ang % 360;
            angle = ang;
        }
        void setImg()
        {
            QLineF temp[3];
            temp[0].setP1(*position);
            temp[0].setAngle(angle);
            temp[0].setLength(13);

            temp[1].setP1(*position);
            temp[1].setAngle(angle + 120);
            temp[1].setLength(8);

            temp[2].setP1(*position);
            temp[2].setAngle(angle - 120);
            temp[2].setLength(8);

            img[0] = QLineF( temp[0].p2(), temp[1].p2() );
            img[1] = QLineF( temp[0].p2(), temp[2].p2() );
            img[2] = QLineF( temp[1].p2(), temp[2].p2() );
        }
    };

    Turtle * turtle;
    Panel * intrPanel; //tutaj beda rysowane poczynania zolwia
    Panel * panel;
    QMap<QString, QString> userBlocks;
public:
    Interpreter(Panel *pn, QWidget *parent = 0);
    ~Interpreter() { delete turtle; }

    void getCommand(QString cmd);
    int checkCommand(const QString& cmd, bool& ok);
    void execCommand(QString& cmd, int arg, bool ok);
    void checkTurtle();
    void checkName(const QString&); // sprawdzenie poprawnosci nazwy bloku
public slots:
    void panelWithTurtle(); // zwraca intrPanel z dorysowanym zolwiem
};

#endif // INTERPRETER_H
