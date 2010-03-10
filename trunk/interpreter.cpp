#include "interpreter.h"
#include <QPainter>
#include <QStringList>

Interpreter::Interpreter(Panel *pn, QWidget *parent) : QWidget(parent)
{
    intrPanel = new Panel;
    panel = pn;
    turtle = new Turtle(new QPointF(intrPanel->pixmap->width() / 2, intrPanel->pixmap->height() / 2));
    turtle->setImg();
}

void Interpreter::getCommand(QString cmd)
{
    try{
        cmd.simplified();  // usuniecie spacji
        cmd = cmd.toUpper(); // litery staja sie duze

        QStringList cmdList = cmd.split(' ', QString::SkipEmptyParts);
        if( cmdList.isEmpty() ) throw QString("Nieprawidlowa skladnia polecenia");

        if(cmdList.front() == "BLOCK")
        {
            if(cmdList.size() < 2 ) throw QString("Blok wymaga nazwy.");
            checkName(cmdList[1]);
            if(cmdList.size() < 4 || cmdList[2] != "[" || cmdList.last() != "]" )
                throw QString("Nieprawidlowe zamkniecie bloku polecen.");

            QString blockName = cmdList[1];
            cmdList.removeFirst(); // usuniecie slowa "BLOCK"
            cmdList.removeFirst(); // usuniecie pola z nazwa bloku
            if( cmdList.contains(blockName) )
                throw QString("Blok nie moze wywolywac sam siebie.");
            QString blockCmds = cmdList.join(" ");
            userBlocks.insert(blockName, blockCmds);
            return;
        }
        if(cmdList.front() == "FOR")
        {
            bool ok = true;
            int i = cmdList[1].toInt(&ok, 10); // pobranie licznika petli
            if(!ok) throw QString("Petla FOR wymaga licznika.");

            cmdList.removeFirst();  // usuwamy "FOR"
            cmdList.removeFirst();  // usuwamy licznik

            QString command;
            command = cmdList.join(" "); // tworzymy string z elementami poodzielanymi spacj¹
            while( --i )
                getCommand(command);
        }
        if( cmdList.front() == "[" )
        {
            if( cmdList.last() != "]" ) throw QString("Blok wymaga zamkniecia.");
            QStringList::Iterator it;
            it = cmdList.begin();
            it++; // ruszamy sie do pierwszego elementu po [
            while( *it != "]" )
            {
                QString command(*it);
                it++;
                if( *it != "]" ) // sprawdzmy czy nie ma po komendzie argumentu dla niej
                {
                    bool ok;
                    try{/*  vv dla komendy przyjmujacej wiecej niz jeden argument ( w tym wypadku dwa )
                        if(command == "?komenda?" && *(it+1) != "]")
                            checkCommand( command + " " + *it + " " + *(it+1), ok );
                        else */
                            checkCommand(command + " " + *it, ok);
                    }catch(...) {}

                    if(ok)
                    {/*  vv dla komendy przyjmujacej wiecej niz jeden argument ( w tym wypadku dwa )
                        if(command == "?komenda?")
                        {
                            command += ( " " + *it + " " + *(it+1) );
                            it++;
                        }
                        else */
                            command += ( " " + *it ); // dodajemy argument do polecenia
                        it++;
                    }
                    if( *it == "["){    // jezeli blok w bloku...
                        while( *it != "]" )
                        {
                            command += ( " " + *it );
                            it++;
                        }
                        if( (it+1) == cmdList.end() )
                            throw QString("Niedomkniety blok.");
                        command += ( " " + *it );
                        it++;
                    }
                }
                getCommand(command);
            }
        }
        else
        {
            bool argOk;
            int arg = 0;
            arg = checkCommand(cmd, argOk);
            if(arg == -1) // jezeli argument komendy jest tekstem
                execCommand(cmd, arg, argOk);
            else // jezeli jest liczba typu int albo go nie ma
                execCommand(cmdList[0], arg, argOk);
        }
    }
    catch( ... )
    {
        throw;
    }
    return;
}
// odzielamy komende od argumentu
int Interpreter::checkCommand(const QString &cmd, bool& jestArg)
{
    QStringList cmdList = cmd.split(' ', QString::SkipEmptyParts);

    /*if( cmdList.count() > 2 && cmdList[0] == "?komenda?" ) <-- blok dla polecen przyjmujacych wiecej niz jeden arg
    {
        for(int i = 1; i < 3; i++)
        {
            cmdList[i].toInt(&jestArg, 10);
            if(!jestArg) throw QString("Podane wspolrzedne sa bledne.");
        }
        return -1;
    }
    else if( cmdList.count() > 2 ) throw QString("Komenda powinna sie skladac z polecenia i ew. argumentu.");
    else if( cmdList[0] == "?komenda?" ) throw QString("Komenda POS wymaga podania dwoch argumentow.");
  */  int arg = 0;
    if( cmdList.count() > 1 )
    {
       /* if( cmdList[0] == "?komenda?" ) // przyklad dla komendy przyjmujacej argument bedacy tekstem
        {
            jestArg = true;
            return -1;
        }*/
        //else{
            jestArg = true;
            arg = cmdList[1].toInt(&jestArg, 10);
            if(!jestArg) throw QString("Podany argument jest bledny."
                                   "\nArgument powinnien byæ liczba typu calkowitego.");
        //}
    }
    else jestArg = false;

    return arg;
}

void Interpreter::execCommand(QString& cmd, int arg, bool jestArg)
{
    if(cmd == "FW" || cmd == "BK")
    {
        if(!jestArg) throw QString("Polecenie " + cmd + " wymaga podania argumentu");

        qreal ang = turtle->angle;
        if(cmd == "BK")
        {
            ang = ((int)turtle->angle + 180) % 360;
            if(ang < 0)
                ang = 360 + ang;
        }

        QLineF line( *turtle->position, QPointF(0, 0) );
        line.setAngle(ang);
        line.setLength( (qreal)arg );

        turtle->position->setX( line.x2() );
        turtle->position->setY( line.y2() );
        checkTurtle();

        if(turtle->down)
            intrPanel->rysujLinie(line, turtle->pen);
    }
    else if(cmd == "RT")
    {
        if(!jestArg) throw QString("Polecenie " + cmd + " wymaga podania argumentu");

        turtle->setAngle( turtle->angle - arg );
    }
    else if(cmd == "LT")
    {
        if(!jestArg) throw QString("Polecenie " + cmd + " wymaga podania argumentu");

        turtle->setAngle( turtle->angle + arg );
    }
    else if(cmd == "ROT")
    {
        if(!jestArg) throw QString("Polecenie " + cmd + " wymaga podania argumentu");

        turtle->setAngle(arg + 90);
    }
    else if(cmd == "UP")
    {
        turtle->down = false;
    }
    else if(cmd == "DOWN")
    {
        turtle->down = true;
    }
    else if(cmd == "RESET")
    {
        turtle->position->setX(intrPanel->pixmap->width() / 2);
        turtle->position->setY(intrPanel->pixmap->height() / 2);
        turtle->setAngle(0);
    }
    // przyklad dla polecenia przyjmujacego wiecej niz jeden argument
   /* else if( cmd.startsWith("?komenda?") )
    {
        cmd.remove("?komend? ");
        // polecenia dla komendy
    }*/
    else if( userBlocks.contains(cmd) )
    {
        getCommand( userBlocks[cmd] );
    }
    else
        throw QString(cmd + ": Nieznane polecenie");
    return;
}

void Interpreter::checkTurtle() // niedajemy ¿ó³wiowi wyjœæ poza nasze pole widzenia
{
    if(turtle->position->x() < 0)
        turtle->position->setX(0);
    if(turtle->position->y() < 0)
        turtle->position->setY(0);
    if(turtle->position->x() > intrPanel->pixmap->width())
        turtle->position->setX(intrPanel->pixmap->width());
    if(turtle->position->y() > intrPanel->pixmap->height())
        turtle->position->setY(intrPanel->pixmap->height());
    return;
}
void Interpreter::panelWithTurtle()
{
    *(panel->pixmap) = intrPanel->pixmap->copy();
    QPainter painter(panel->pixmap);

    turtle->setImg();
    painter.drawLines( turtle->img, 3 );

    return;
}

void Interpreter::checkName(const QString& str)
{
    if(str == "RT" || str == "LT" || str == "ROT" || str == "UP" ||
       str == "DOWN" || str == "RESET" || str == "BK" || str == "FW")
        throw QString("Ta nazwa jest juz zarezerwowana. \nWymysl wlasna.");
    int i = 0;
    while( i < str.size() )
    {
        if(str[i] > 'Z' || str[i] < 'A')
            throw QString("Nazwa powinna skladac sie tylko z liter.");
        i++;
    }
    return;
}
