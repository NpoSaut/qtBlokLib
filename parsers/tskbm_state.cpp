#include "tskbm_state.h"

void TskbmState::getCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x5801 ) // id: 0x2c0
    {
        setOnline(            frame[0] & (1 << 7) > 0);
        setMachinistCheerful( frame[0] & (1 << 6) > 0);
        setVigilanceRequest(  frame[0] & (1 << 5) > 0);
        setPreAlarm(          frame[0] & (1 << 4) > 0);
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
