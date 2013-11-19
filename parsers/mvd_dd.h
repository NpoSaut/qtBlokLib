#ifndef MVD_DD_H
#define MVD_DD_H

#include "canblokmessage.h"
#include "pressure.h"

// MVD_DD
// id: 0x2EA
// len: 5
// desc: 0x5D45
class MvdDd : public PeriodicalCanBlokMessage
{
    Q_OBJECT

public:
    explicit MvdDd(QObject *parent = 0);

    const Pressure &getTcPressure () const { return tcPressure; }
    const Pressure &getTmPressure () const { return tmPressure; }

signals:
    void tcPressureChanged (Pressure value);
    void tmPressureChanged (Pressure value);

public slots:
    bool setTcPressure (Pressure value);
    bool setTmPressure (Pressure value);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    Pressure tcPressure;
    Pressure tmPressure;
};

#endif // MVD_DD_H
