#include "uktol_dd1.h"

UktolDd1::UktolDd1(QObject *parent) :
    PeriodicalCanBlokMessage(0x1E6, 7, parent),
    urPressure(),
    tcPressure(),
    tmPressure()
{
}

void UktolDd1::fillMessage(CanFrame &frame) const
{
    frame[1] =
              (quint8 (tmPressure.getValidity ()) << 2)
            | (quint8 (tcPressure.getValidity ()) << 1)
            | (quint8 (urPressure.getValidity ()) << 0);

    quint16 urConvert = urPressure.getAtm () * 100;
    frame[2] = quint8(urConvert);
    frame[3] = quint8(urConvert/256);

    quint16 tcConvert = tcPressure.getAtm () * 100;
    frame[4] = quint8(tcConvert);
    frame[5] = quint8(tcConvert/256);

    quint16 tmConvert = tmPressure.getAtm () * 100;
    frame[6] = quint8(tmConvert);
    frame[7] = quint8(tmConvert/256);
}

bool UktolDd1::setUrPressure(Pressure value)
{
    if ( urPressure != value || theFirstTime )
    {
        urPressure = value;
        emit urPressureChanged (urPressure);
        return true;
    }
    return false;
}

bool UktolDd1::setTcPressure(Pressure value)
{
    if ( tcPressure != value )
    {
        tcPressure = value;
        emit tcPressureChanged (tcPressure);
        return true;
    }
    return false;
}

bool UktolDd1::setTmPressure(Pressure value)
{
    if ( tmPressure != value )
    {
        tmPressure = value;
        emit tmPressureChanged (tmPressure);
        return true;
    }
    return false;
}

bool UktolDd1::parseSuitableMessage(const CanFrame &frame)
{
    Pressure ur, tc, tm;

    ur.setValidity ( frame[1] & (1 << 0) );
    tc.setValidity ( frame[1] & (1 << 1) );
    tm.setValidity ( frame[1] & (1 << 2) );

    ur.setAtm ( quint16(frame[3]) + quint16(frame[2]) );
    tc.setAtm ( quint16(frame[5]) + quint16(frame[4]) );
    tm.setAtm ( quint16(frame[7]) + quint16(frame[6]) );

    return
            setUrPressure (ur)
         || setTcPressure (tc)
         || setTmPressure (tm);
}

