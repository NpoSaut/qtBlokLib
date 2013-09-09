#ifndef CANBLOKMESSAGE_H
#define CANBLOKMESSAGE_H

#include <QObject>

#include "qtCanLib/canframe.h"

class Parser;

class CanBlokMessage : public QObject
{
    Q_OBJECT
    friend class Parser;
public:
    explicit CanBlokMessage (QObject *parent = 0);

private slots:
    virtual void getCanMessage (CanFrame canFrame) = 0;

protected:
    bool theFirstTime;
};

#endif // CANBLOKMESSAGE_H
