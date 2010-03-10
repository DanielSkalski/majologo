#include "panel.h"


Panel::Panel()
{
    pixmap = new QPixmap(600, 600);
    pixmap->fill( QColor(255, 255, 255) );
    update();
}


void Panel::paintEvent( QPaintEvent * )
{
    QPainter paint(this);
    paint.drawPixmap(0, 0, *pixmap);
}

void Panel::mousePressEvent( QMouseEvent * e )
{

}
