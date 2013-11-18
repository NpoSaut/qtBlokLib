#include "display_state.h"

#include <QMetaType>

DisplayStateA::DisplayStateA(QObject *parent) :
    CanBlokMessage(0x28F, 3, parent),
    rb (false), rbs (false), vk (false), mode (TRAIN), pull (false), otpr (false), oc (false), k20 (false), freq (false)
{
    qRegisterMetaType<DriveMode> ("DriveMode");
}

void DisplayStateA::fillMessage(CanFrame &frame) const
{
    frame[1] = 0; // Нет ошибок
    frame[2] = ( (quint8)isRbPressed ()     << 7 )
             | ( (quint8)isRbsPressed ()    << 6 )
             | ( (quint8)isVkPressed ()     << 5 )
             | ( (getDriveMode () == SHUNTING) << 4 )
             | ( (quint8)isPullPressed ()   << 3 )
             | ( (quint8)isOtprPressed ()   << 2 )
             | ( (quint8)isOcPressed ()     << 1 )
             | ( (quint8)isK20Pressed ()    << 0 );
    frame[3] = ( (quint8)isFreqPressed ()    << 7 )
            | ( (getDriveMode () == WORKING)  << 6 );
}

bool DisplayStateA::setRb(bool press)
{
    if ( rb != press || theFirstTime )
    {
        rb = press;
        emit rbChanged (rb);
        return true;
    }
    return false;
}

bool DisplayStateA::setRbs(bool press)
{
    if ( rbs != press || theFirstTime )
    {
        rbs = press;
        emit rbsChanged (rbs);
        return true;
    }
    return false;
}

bool DisplayStateA::setVk(bool press)
{
    if ( vk != press || theFirstTime )
    {
        vk = press;
        emit vkChanged (vk);
        return true;
    }
    return false;
}

bool DisplayStateA::setDriveMode(DriveMode newMode)
{
    if ( mode != newMode || theFirstTime )
    {
        mode = newMode;
        emit driveModeChanged (mode);
        return true;
    }
    return false;
}

bool DisplayStateA::setPull(bool press)
{
    if ( pull != press || theFirstTime )
    {
        pull = press;
        emit pullChanged (pull);
        return true;
    }
    return false;
}

bool DisplayStateA::setOtpr(bool press)
{
    if ( otpr != press || theFirstTime )
    {
        otpr = press;
        emit otprChanged (otpr);
        return true;
    }
    return false;
}

bool DisplayStateA::setOc(bool press)
{
    if ( oc != press || theFirstTime )
    {
        oc = press;
        emit ocChanged (oc);
        return true;
    }
    return false;
}

bool DisplayStateA::setK20(bool press)
{
    if ( k20 != press || theFirstTime )
    {
        k20 = press;
        emit k20Changed (k20);
        return true;
    }
    return false;
}

bool DisplayStateA::setFreq(bool press)
{
    if ( freq != press || theFirstTime )
    {
        freq = press;
        emit freqChanged (freq);
        return true;
    }
    return false;
}

bool DisplayStateA::parseSuitableMessage(const CanFrame &frame)
{
    bool update =
               setRb       (frame[2] & (1 << 7))
            || setRbs      (frame[2] & (1 << 6))
            || setVk       (frame[2] & (1 << 5))
            || setPull     (frame[2] & (1 << 3))
            || setOtpr     (frame[2] & (1 << 2))
            || setOc       (frame[2] & (1 << 1))
            || setK20      (frame[2] & (1 << 0))
            || setFreq     (frame[3] & (1 << 7));

    if (frame[3] & (1 << 6))
        update = update || setDriveMode (WORKING);
    else if (frame[2] & (1 << 4))
        update = update || setDriveMode (SHUNTING);
    else
        update = update || setDriveMode (TRAIN);

    return update;
}

DisplayStateB::DisplayStateB(QObject *parent) :
    CanBlokMessage (0x29F, 2, parent),
    rb (false), rbs (false)
{
}

void DisplayStateB::fillMessage(CanFrame &frame) const
{
    frame[1] = 0;
    frame[2] = (quint8 (isRbPressed ()) << 7)
            | (quint8 (isRbsPressed ()) << 6);
}

bool DisplayStateB::setRb(bool pressed)
{
    if ( rb != pressed || theFirstTime )
    {
        rb = pressed;
        emit rbChanged (rb);
        return true;
    }
    return false;
}

bool DisplayStateB::setRbs(bool pressed)
{
    if ( rbs != pressed || theFirstTime )
    {
        rbs = pressed;
        emit rbsChanged (rbs);
        return true;
    }
    return false;
}

bool DisplayStateB::parseSuitableMessage(const CanFrame &frame)
{
    return
            setRb (frame[2] & (1 << 7))
         || setRbs(frame[2] & (1 << 6));
}

