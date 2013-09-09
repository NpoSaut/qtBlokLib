#ifndef ELMAPFORWARDTARGET_H
#define ELMAPFORWARDTARGET_H

#include <QObject>
#include <QString>
#include "qtCanLib/can.h"
#include "qtCanLib/canframe.h"

class ElmapForwardTarget : public QObject
{
    Q_OBJECT
public:
    explicit ElmapForwardTarget(Can *onCan, QObject *parent = 0);

    QString getName () const { return name; }
    int getDistance () const { return distance; }
    int getKind () const { return kind; }

signals:
    void nameChanged (QString name);
    void distanceChanged (int distance);
    void kindChanged (int kind);
    
public slots:
    void getNameFromMmSignal (CanFrame mmSignal);
    void getDistanceFromMcoState (CanFrame mcoState);
    void getKindFromMcoLimits (CanFrame mcoLimits);

private:
    Can *can;
    QString name;
    int distance;
    int kind;
};

#endif // ELMAPFORWARDTARGET_H
