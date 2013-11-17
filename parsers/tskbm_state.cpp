#include "tskbm_state.h"

#include <QDebug>

TskbmState::TskbmState(QObject *parent)
    : CanBlokMessage (parent),
      online (false), machinistCheerful (false), vigilanceRequest (false), preAlarm (false)
{
}

CanFrame TskbmState::encode() const
{
    CanFrame frame (0x5801);
    frame[1] = (quint8 (isOnline ()) << 7)
            | (quint8 (isMachinistCheerful ()) << 6)
            | (quint8 (isVigilanceRequest ()) << 5)
            | (quint8 (isPreAlarm ()) << 4);
    return frame;
}

void TskbmState::setOnline(bool newValue)
{
    if (newValue != online || theFirstTime)
    {
        online = newValue;
        emit onlineChanged(newValue);
        emit whateverChanged();
    }
}
void TskbmState::setMachinistCheerful(bool newValue)
{
    if (newValue != machinistCheerful || theFirstTime)
    {
        machinistCheerful = newValue;
        emit machinistCheerfulChanged(newValue);
        emit whateverChanged();
    }
}
void TskbmState::setVigilanceRequest(bool newValue)
{
    if (newValue != vigilanceRequest || theFirstTime)
    {
        vigilanceRequest = newValue;
        emit vigilanceRequestChanged(newValue);
        emit whateverChanged();
    }
}
void TskbmState::setPreAlarm(bool newValue)
{
    if (newValue != preAlarm || theFirstTime)
    {
        preAlarm = newValue;
        emit preAlarmChanged(newValue);
        emit whateverChanged();
    }
}

void TskbmState::processCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x5801 ) // id: 0x2c0
    {
        setOnline(            (frame[1] & (1 << 7)) );
        setMachinistCheerful( (frame[1] & (1 << 6)) );
        setVigilanceRequest(  (frame[1] & (1 << 5)) );
        setPreAlarm(          (frame[1] & (1 << 4)) );

        if (theFirstTime)
            theFirstTime = false;

        emit messageReceived ();
    }
}


