#ifndef PARSER_H
#define PARSER_H

#include <QObject>

#include "qtCanLib/can.h"
#include "qtCanLib/canframe.h"

class Parser;

class CanBlokMessage : public QObject
{
    Q_OBJECT
    friend class Parser;
public:
    explicit CanBlokMessage (Can *onCan, QObject *parent = 0)
        : QObject (parent), can(onCan)
    {}
private:
    Can *can;
private slots:
    virtual void getCanMessage (CanFrame canFrame) = 0;
};

class MmCoord : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit MmCoord (Can *onCan, QObject *parent = 0)
        : CanBlokMessage (onCan, parent)
    {}

signals:
    void railWayCoordinateChanged (int coordinate);

private slots:
    void getCanMessage (CanFrame frame);

private:
    int coord;
};

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(Can *onCan, QObject *parent = 0);

    MmCoord mmCoord;
    
signals:
    
public slots:
    
};

#endif // PARSER_H
