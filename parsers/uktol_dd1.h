#ifndef UKTOL_DD1_H
#define UKTOL_DD1_H

#include "canblokmessage.h"
#include "pressure.h"

// UKTOL_DD1
// id: 0x1E6
// len: 7
// desc: 0x3CC7
class UktolDd1 : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit UktolDd1(QObject *parent = 0);

    Pressure getUrPressure () const { return urPressure; }
    Pressure getTcPressure () const { return tcPressure; }
    Pressure getTmPressure () const { return tmPressure; }

signals:
    void urPressureChanged (Pressure value);
    void tcPressureChanged (Pressure value);
    void tmPressureChanged (Pressure value);
    
public slots:
    bool setUrPressure (Pressure value);
    bool setTcPressure (Pressure value);
    bool setTmPressure (Pressure value);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    Pressure urPressure;
    Pressure tcPressure;
    Pressure tmPressure;
};

#endif // UKTOL_DD1_H
