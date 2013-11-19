#include "vds_state.h"

#include <QMetaType>

VdsState::VdsState(QObject *parent) :
    PeriodicalCanBlokMessage(0x2E0, 2, parent),
    epvKey (false),
    traction (false),
    transmissionDirection(NEUTRAL),
    engineWork (false),
    vigilanceButton (false),
    emergencyStop (false),
    siren (false),
    tifon (false),
    ironWheels (true)
{
    qRegisterMetaType<TransmissionDirection>("TransmissionDirection");
}

void VdsState::fillMessage(CanFrame &frame) const
{
    frame[1] = (qint8 (isEpvKey ()) << 2)
            | (qint8 (!isTraction ()) << 0);
    frame[2] = (qint8 (getTransmissionDirection ()) << 6)
            | (qint8 (isEngineWork ()) << 5)
            | (qint8 (isVigilanceButton ()) << 4)
            | (qint8 (isEmergencyStop ()) << 3)
            | (qint8 (isSiren ()) << 2)
            | (qint8 (isTifon ()) << 1)
            | (qint8 (isIronWheels ()) << 0);
}


bool VdsState::setEpvKey(bool key)
{
    if (epvKey != key || theFirstTime)
    {
        epvKey = key;
        emit epvKeyChanged (epvKey);
        return true;
    }
    return false;
}

bool VdsState::setTraction(bool tr)
{
    if (traction != tr || theFirstTime)
    {
        traction = tr;
        emit tractionChanged (traction);
        return true;
    }
    return false;
}

bool VdsState::setTransmissionDirection(VdsState::TransmissionDirection td)
{
    if (transmissionDirection != td || theFirstTime)
    {
        transmissionDirection = td;
        emit transmissionDirectionChanged (transmissionDirection);
        return true;
    }
    return false;
}

bool VdsState::setEngineWork(bool ew)
{
    if (engineWork != ew || theFirstTime)
    {
        engineWork = ew;
        emit engineWorkChanged (engineWork);
        return true;
    }
    return false;
}

bool VdsState::setVigilanceButton(bool vb)
{
    if (vigilanceButton != vb || theFirstTime)
    {
        vigilanceButton = vb;
        emit vigilanceButtonChanged (vigilanceButton);
        return true;
    }
    return false;
}

bool VdsState::setEmergencyStop(bool stop)
{
    if (emergencyStop != stop || theFirstTime)
    {
        emergencyStop = stop;
        emit emergencyStopChanged (emergencyStop);
        return true;
    }
    return false;
}

bool VdsState::setSiren(bool sn)
{
    if (siren != sn || theFirstTime)
    {
        siren = sn;
        emit sirenChanged (siren);
        return true;
    }
    return false;
}

bool VdsState::setTifon(bool tf)
{
    if (tifon != tf || theFirstTime)
    {
        tifon = tf;
        emit tifonChanged (tifon);
        return true;
    }
    return false;
}

bool VdsState::setIronWheels(bool iw)
{
    if (ironWheels != iw || theFirstTime)
    {
        ironWheels = iw;
        emit ironWheelsChagned (ironWheels);
        return true;
    }
    return false;
}

bool VdsState::parseSuitableMessage(const CanFrame &frame)
{
    return
        setEpvKey                   (frame[1] & (1 << 2))
     || setTraction               (!(frame[1] & (1 << 0)))
     || setTransmissionDirection  (TransmissionDirection((frame[2] >> 6) & 0x3))
     || setEngineWork               (frame[2] & (1 << 5))
     || setVigilanceButton          (frame[2] & (1 << 4))
     || setEmergencyStop            (frame[2] & (1 << 3))
     || setSiren                    (frame[2] & (1 << 2))
     || setTifon                    (frame[2] & (1 << 1))
     || setIronWheels               (frame[2] & (1 << 0));
}
