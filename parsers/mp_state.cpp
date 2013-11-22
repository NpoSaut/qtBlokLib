#include "mp_state.h"

#include "QMetaType"

MpState::MpState(QObject *parent) :
    CanBlokMessage(0x08F, 8, parent),
    trafficlight (WHITE),
    deviationFromNarrow (false),
    targetSpeed (0),
    restrictionSpeed (0),
    kptType (KPT_UNKNOWN),
    frequency (FREQ25),
    alsnActive (false),
    alsEnActive (false),
    autoblockClosed (false),
    autoblockSemiClosed (false),
    blockzoneLength (0)
{
    qRegisterMetaType<Trafficlight>("Trafficlight");
    qRegisterMetaType<KptType>("KptType");
    qRegisterMetaType<AlsnFrequency>("AlsnFrequency");
}

void MpState::fillMessage(CanFrame &frame) const
{
    frame[1] = 0;
    frame[2] = ( quint8 (getTrafficlight ()) << 4 )
            | ( quint8(!isDeviationFromNarrow ()) << 3 )
            | ( quint8 (isDeviationFromNarrow ()) << 2 )
            | ( (quint8 (getTargetSpeed ()/256) & 1) << 1 )
            | ( (quint8 (getRestrictionSpeed ()/256) & 1) << 0 );
    frame[3] = quint8 (getRestrictionSpeed ());
    frame[4] = 0; // Синхрогруппа и кодовая кобинация. Недоумеваю.
    frame[5] = ( (quint8 (getKptType ()) & 0x3) << 6 )
            | ( (quint8 (getFrequense ()) & 0x3) << 4 )
            | ( quint8 (isAlsnActive ()) << 3 )
            | ( quint8 (isAlsEnActive ()) << 2 )
            | ( quint8 (getAlsnLight ()) & 0x3 );
    frame[6] = ( quint8 (isAutoblockClosed ()) << 6 )
            | ( quint8 (isAutoblockSemiClosed ()) << 5 )
            | ( (quint8 (getBlockzoneLength ()/256) & 0x1F) << 0 );
    frame[7] = quint8 (getBlockzoneLength ());
    frame[8] = quint8 (getTargetSpeed ());
}

bool MpState::setTrafficlight(Trafficlight tl)
{
    if ( trafficlight != tl || theFirstTime )
    {
        trafficlight = tl;
        emit trafficlightChanged (trafficlight);
        return true;
    }
    return false;
}

bool MpState::setDeviationFromNarrow(bool deviaton)
{
    if ( deviationFromNarrow != deviaton || theFirstTime )
    {
        deviationFromNarrow = deviaton;
        emit deviationFromNarrowChanged (deviationFromNarrow);
        return true;
    }
    return false;
}

bool MpState::setTargetSpeed(int speed)
{
    if ( targetSpeed != speed || theFirstTime )
    {
        targetSpeed = speed;
        targetSpeedChanged (targetSpeed);
        return true;
    }
    return false;
}

bool MpState::setRestrictionSpeed(int speed)
{
    if ( restrictionSpeed != speed || theFirstTime )
    {
        restrictionSpeed = speed;
        restrictionSpeedChanged (restrictionSpeed);
        return true;
    }
    return false;
}

bool MpState::setKptType(KptType type)
{
    if ( kptType != type || theFirstTime )
    {
        kptType = type;
        kptTypeChanged (kptType);
        return true;
    }
    return false;
}

bool MpState::setFrequency(AlsnFrequency freq)
{
    if ( frequency != freq || theFirstTime )
    {
        frequency = freq;
        frequencyChanged (frequency);
        return true;
    }
    return false;
}

bool MpState::setAlsnActive(bool active)
{
    if ( alsnActive != active || theFirstTime )
    {
        alsnActive = active;
        emit alsnActiveChanged (alsnActive);
        return true;
    }
    return false;
}

bool MpState::setAlsEnActive(bool active)
{
    if ( alsEnActive != active || theFirstTime )
    {
        alsEnActive = active;
        emit alsEnActiveChanged (alsEnActive);
        return true;
    }
    return false;
}

bool MpState::setAlsnLight(AlsnLight light)
{
    if ( alsnLight != light || theFirstTime )
    {
        alsnLight = light;
        emit alsnLightChanged (alsnLight);
        return true;
    }
    return false;
}

bool MpState::setAutoblockClosed(bool closed)
{
    if ( autoblockClosed != closed || theFirstTime )
    {
        autoblockClosed = closed;
        emit autoblockClosedChanged (autoblockClosed);
        return true;
    }
    return false;
}

bool MpState::setAutoblockSemiClosed(bool semiClosed)
{
    if ( autoblockSemiClosed != semiClosed || theFirstTime )
    {
        autoblockSemiClosed = semiClosed;
        autoblockSemiClosedChanged (autoblockSemiClosed);
        return true;
    }
    return false;
}

bool MpState::setBlockzoneLength(int length)
{
    if ( blockzoneLength != length || theFirstTime )
    {
        blockzoneLength = length;
        blockzoneLengthChanged (blockzoneLength);
        return true;
    }
    return false;
}

bool MpState::parseSuitableMessage(const CanFrame &frame)
{
    return
            setTrafficlight (Trafficlight (frame[2] >> 4))
            || setDeviationFromNarrow (frame[2] & (1 << 3))
            || setRestrictionSpeed ( quint16 (frame[3]) + quint16 (frame[2] & (1 << 0))*256 )
            || setTargetSpeed ( quint16 (frame[8]) + quint16 (frame[2] >> 1 & 1)*256 )
            || setKptType (KptType (frame[5] >> 6))
            || setFrequency (AlsnFrequency ((frame[5] >> 4) & 0x3))
            || setAlsnActive (frame[5] & (1 << 3))
            || setAlsEnActive (frame[5] & (1 << 2))
            || setAlsnLight ( AlsnLight (frame[5] & 0x3) )
            || setAutoblockClosed (frame[6] & (1 << 6))
            || setAutoblockSemiClosed (frame[6] & (1 << 5))
            || setBlockzoneLength ( quint16 (frame[7]) + quint16 (frame[6] & 0xF)*256 );
}


