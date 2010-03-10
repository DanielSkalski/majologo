#ifndef PANEL_H
#define PANEL_H

#include <QWidget>
#include <QPixmap>
#include <QColor>
#include <QPainter>
#include <QLineF>
#include <QMouseEvent>
#include <QMenu>

class Panel : public QWidget
{
    Q_OBJECT
friend class MainWindow;
friend class mainWidget;
friend class Interpreter;
    QPixmap *pixmap;
protected:

    void mousePressEvent( QMouseEvent* );
    void paintEvent( QPaintEvent *);
    void rysujLinie( QLineF linia, QPen pen )
    {
        QPainter paint(pixmap);
        paint.setPen(pen);
        paint.drawLine(linia);
        update();
    }
public:
    Panel();
};

#endif // PANEL_H
