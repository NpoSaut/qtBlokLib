#include "mvd_dd.h"

MvdDd::MvdDd(QObject *parent) :
    PeriodicalCanBlokMessage(0x2EA, 5, parent),
    tcPressure(),
    tmPressure()
{
}

void MvdDd::fillMessage(CanFrame &frame) const
{
    frame[1] = (quint8 (!tmPressure.isValid ()) << 1)
             | (quint8 (!tcPressure.isValid ()) << 0);
    frame[2] = quint8 (tcPressure.getMpa () * 255);
    frame[2] = quint8 (tmPressure.getMpa () * 255);
    frame[4] = 0;
    frame[5] = 0;
}

bool MvdDd::setTcPressure(Pressure value)
{
    if ( tcPressure != value )
    {
        tcPressure = value;
        emit tcPressureChanged (tcPressure);
        return true;
    }
    return false;
}

bool MvdDd::setTmPressure(Pressure value)
{
    if ( tmPressure != value )
    {
        tmPressure = value;
        emit tmPressureChanged (tmPressure);
        return true;
    }
    return false;
}

bool MvdDd::parseSuitableMessage(const CanFrame &frame)
{
    Pressure tc, tm;

    tc.setValid ( !(frame[1] & (1 << 0)) );
    tm.setValid ( !(frame[1] & (1 << 1)) );

    tc.setMpa ( float(frame[2]) / 255 );
    tm.setMpa ( float(frame[3]) / 255 );

    return
        setTcPressure (tc)
      | setTmPressure (tm);
}
