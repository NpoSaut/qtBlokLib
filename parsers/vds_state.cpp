#include "vds_state.h"

#include <QMetaType>

VdsState::VdsState(QObject *parent) :
    CanBlokMessage(parent)
{
    qRegisterMetaType<TransmissionDirection>("TransmissionDirection");
}

CanFrame VdsState::encode() const
{
    CanFrame frame (0x5C02);
    frame[1] = (qint8 (isEpvKey ()) << 2)
            | (qint8 (!isTraction ()) << 0);
    frame[2] = (qint8 (getTransmissionDirection ()) << 6)
            | (qint8 (isEngineWork ()) << 5)
            | (qint8 (isVigilanceButton ()) << 4)
            | (qint8 (isEmergencyStop ()) << 3)
            | (qint8 (isSiren ()) << 2)
            | (qint8 (isTifon ()) << 1)
            | (qint8 (isIronWheels ()) << 0);
    return frame;
}


void VdsState::setEpvKey(bool key)
{
    if (epvKey != key || theFirstTime)
    {
        epvKey = key;
        emit epvKeyChanged (epvKey);
        emit whateverChanged ();
    }
}

void VdsState::setTraction(bool tr)
{
    if (traction != tr || theFirstTime)
    {
        traction = tr;
        emit tractionChanged (traction);
        emit whateverChanged ();
    }
}

void VdsState::setTransmissionDirection(VdsState::TransmissionDirection td)
{
    if (transmissionDirection != td || theFirstTime)
    {
        transmissionDirection = td;
        emit transmissionDirectionChanged (transmissionDirection);
        emit whateverChanged ();
    }
}

void VdsState::setEngineWork(bool ew)
{
    if (engineWork != ew || theFirstTime)
    {
        engineWork = ew;
        emit engineWorkChanged (engineWork);
        emit whateverChanged ();
    }
}

void VdsState::setVigilanceButton(bool vb)
{
    if (vigilanceButton != vb || theFirstTime)
    {
        vigilanceButton = vb;
        emit vigilanceButtonChanged (vigilanceButton);
        emit whateverChanged ();
    }
}

void VdsState::setEmergencyStop(bool stop)
{
    if (emergencyStop != stop || theFirstTime)
    {
        emergencyStop = stop;
        emit emergencyStopChanged (emergencyStop);
        emit whateverChanged ();
    }
}

void VdsState::setSiren(bool sn)
{
    if (siren != sn || theFirstTime)
    {
        siren = sn;
        emit sirenChanged (siren);
        emit whateverChanged ();
    }
}

void VdsState::setTifon(bool tf)
{
    if (tifon != tf || theFirstTime)
    {
        tifon = tf;
        emit tifonChanged (tifon);
        emit whateverChanged ();
    }
}

void VdsState::setIronWheels(bool iw)
{
    if (ironWheels != iw || theFirstTime)
    {
        ironWheels = iw;
        emit ironWheelsChagned (ironWheels);
        emit whateverChanged ();
    }
}

void VdsState::processCanMessage(CanFrame frame)
{
    if (frame.getDescriptor () == 0x5C02)
    {
        setEpvKey                   (frame[1] & (1 << 2));
        setTraction               (!(frame[1] & (1 << 0)));
        setTransmissionDirection  (TransmissionDirection((frame[2] >> 6) & 0x3));
        setEngineWork               (frame[2] & (1 << 5));
        setVigilanceButton          (frame[2] & (1 << 4));
        setEmergencyStop            (frame[2] & (1 << 3));
        setSiren                    (frame[2] & (1 << 2));
        setTifon                    (frame[2] & (1 << 1));
        setIronWheels               (frame[2] & (1 << 0));

        if (theFirstTime)
            theFirstTime = false;

        emit messageReceived ();
    }
}
