#include "tskbm_state.h"

#include <QDebug>

TskbmState::TskbmState(QObject *parent)
    : PeriodicalCanBlokMessage (0x2C0, 1, parent),
      online (false), machinistCheerful (false), vigilanceRequest (false), preAlarm (false)
{
}

void TskbmState::fillMessage(CanFrame &frame) const
{
    frame[1] = (quint8 (isOnline ()) << 7)
            | (quint8 (isMachinistCheerful ()) << 6)
            | (quint8 (isVigilanceRequest ()) << 5)
            | (quint8 (isPreAlarm ()) << 4);
}

bool TskbmState::setOnline(bool newValue)
{
    if (newValue != online || theFirstTime)
    {
        online = newValue;
        emit onlineChanged(newValue);
        return true;
    }
    return false;
}

bool TskbmState::setMachinistCheerful(bool newValue)
{
    if (newValue != machinistCheerful || theFirstTime)
    {
        machinistCheerful = newValue;
        emit machinistCheerfulChanged(newValue);
        return true;
    }
    return false;
}

bool TskbmState::setVigilanceRequest(bool newValue)
{
    if (newValue != vigilanceRequest || theFirstTime)
    {
        vigilanceRequest = newValue;
        emit vigilanceRequestChanged(newValue);
        return true;
    }
    return false;
}

bool TskbmState::setPreAlarm(bool newValue)
{
    if (newValue != preAlarm || theFirstTime)
    {
        preAlarm = newValue;
        emit preAlarmChanged(newValue);
        return true;
    }
    return false;
}

bool TskbmState::parseSuitableMessage(const CanFrame &frame)
{
    return
        setOnline(            (frame[1] & (1 << 7)) )
     || setMachinistCheerful( (frame[1] & (1 << 6)) )
     || setVigilanceRequest(  (frame[1] & (1 << 5)) )
     || setPreAlarm(          (frame[1] & (1 << 4)) );
}


