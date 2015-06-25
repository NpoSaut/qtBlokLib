#ifndef ELMAPFORWARDTARGET_H
#define ELMAPFORWARDTARGET_H

#include <QObject>
#include <QString>
#include "qtCanLib/ICan.h"

class ElmapForwardTarget : public QObject
{
    Q_OBJECT
public:
    explicit ElmapForwardTarget(ICan *onCan, QObject *parent = 0);

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
    ICan *can;
    QString name;
    int distance;
    int kind;
};

#endif // ELMAPFORWARDTARGET_H
