#ifndef PARSER_H
#define PARSER_H

#include <QObject>

#include "qtCanLib/canframe.h"

class Parser;

class CanBlokMessage : public QObject
{
    Q_OBJECT
    friend class Parser;
public:
    explicit CanBlokMessage (QObject *parent = 0)
        : QObject (parent)
    {}

private slots:
    virtual void getCanMessage (CanFrame canFrame) = 0;
};

class MmCoord : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit MmCoord (QObject *parent = 0)
        : CanBlokMessage (parent)
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
    explicit Parser(QObject *parent = 0);

    MmCoord mmCoord;
    
signals:
    
public slots:
    
};

extern Parser blokMessages;

#endif // PARSER_H
