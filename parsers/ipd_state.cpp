#include "ipd_state.h"

IpdState::IpdState(QObject *parent) :
    CanBlokMessage(parent),
    inMotion(false)
{
}

void IpdState::getCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x1888 ) // id: 0x0C4
    {
        bool newInMotion = frame[2] & (1 << 2);

        if ( newInMotion != inMotion )
        {
            inMotion = newInMotion;
            emit inMotionChanged (inMotion);
            emit whateverChanged ();
        }

        if ( theFirstTime )
        {
            emit inMotionChanged (inMotion);
            emit whateverChanged ();
        }
    }
}
