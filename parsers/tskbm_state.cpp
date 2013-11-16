#include "tskbm_state.h"

#include <QDebug>

TskbmState::TskbmState(QObject *parent)
    : CanBlokMessage (parent),
      online (false), machinistCheerful (false), vigilanceRequest (false), preAlarm (false)
{
}

void TskbmState::getCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x5801 ) // id: 0x2c0
    {
        setOnline(            (frame[1] & (1 << 7)) );
        setMachinistCheerful( (frame[1] & (1 << 6)) );
        setVigilanceRequest(  (frame[1] & (1 << 5)) );
        setPreAlarm(          (frame[1] & (1 << 4)) );
    }
}

void TskbmState::setOnline(bool newValue)
{
    if (newValue != online)
    {
        online = newValue;
        emit onlineChanged(newValue);
        emit whateverChanged();
    }
}
void TskbmState::setMachinistCheerful(bool newValue)
{
    if (newValue != machinistCheerful)
    {
        machinistCheerful = newValue;
        emit machinistCheerfulChanged(newValue);
        emit whateverChanged();
    }
}
void TskbmState::setVigilanceRequest(bool newValue)
{
    if (newValue != vigilanceRequest)
    {
        vigilanceRequest = newValue;
        emit vigilanceRequestChanged(newValue);
        emit whateverChanged();
    }
}
void TskbmState::setPreAlarm(bool newValue)
{
    if (newValue != preAlarm)
    {
        preAlarm = newValue;
        emit preAlarmChanged(newValue);
        emit whateverChanged();
    }
}

