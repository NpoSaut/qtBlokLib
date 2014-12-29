#include "ipd_state.h"

IpdState::IpdState(QObject *parent) :
    PeriodicalCanBlokMessage(0x0C4, 8, parent),
    inMotion(false),
    activeDps(-1)
{
}

void IpdState::fillMessage(CanFrame &frame) const
{
    frame[1] = 0;
    frame[2] = (qint8 (isInMotion ()) << 2);
    frame[3] = 0;
    frame[4] = 0;
    frame[5] = 0;
    frame[6] = 0;
    frame[7] = ( ((getActiveDpsNumber()-1)&1) << 3 );
    frame[8] = 0;
}

bool IpdState::setInMotion(bool motion)
{
    if ( inMotion != motion || theFirstTime )
    {
        inMotion = motion;
        emit inMotionChanged (inMotion);
        return true;
    }
    return false;
}

bool IpdState::setActiveDpsNumber(int activeDpsNumber)
{
    if ( activeDps != activeDpsNumber || theFirstTime )
    {
        activeDps = activeDpsNumber;
        emit activeDpsNumberChanged (activeDps);
        return true;
    }
    return false;
}

bool IpdState::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;
    update = setInMotion (frame[2] & (1 << 2)) || update;
    update = setActiveDpsNumber( ((frame[7] & (1 << 3)) >> 3) + 1 ) || update;
    return update;
}

