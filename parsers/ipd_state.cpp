#include "ipd_state.h"

IpdState::IpdState(QObject *parent) :
    CanBlokMessage(parent),
    inMotion(false)
{
}

CanFrame IpdState::encode() const
{
    CanFrame frame (0x1888);
    frame[1] = 0;
    frame[2] = (qint8 (isInMotion ()) << 2);
    frame[3] = 0;
    frame[4] = 0;
    frame[5] = 0;
    frame[6] = 0;
    frame[7] = 0;
    frame[8] = 0;
}

void IpdState::setInMotion(bool motion)
{
    if ( inMotion != motion || theFirstTime )
    {
        inMotion = motion;
        emit inMotionChanged (inMotion);
        emit whateverChanged ();
    }
}

void IpdState::processCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x1888 ) // id: 0x0C4
    {
        setInMotion (frame[2] & (1 << 2));

        if ( theFirstTime )
            theFirstTime = false;

        emit messageReceived ();
    }
}

