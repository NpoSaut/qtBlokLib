#include "ipd_state.h"

IpdState::IpdState(QObject *parent) :
    CanBlokMessage(0x0C4, 8, parent),
    inMotion(false)
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
    frame[7] = 0;
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



bool IpdState::parseSuitableMessage(const CanFrame &frame)
{
    return
        setInMotion (frame[2] & (1 << 2));
}

