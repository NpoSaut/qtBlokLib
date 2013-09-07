#include "mco_state.h"

McoState::McoState(QObject *parent) :
    CanBlokMessage(parent),
    epvReady (false),
    epvReleased (false)
{
}


void McoState::getCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x0A08 ) // id: 0x050
    {
        bool newEpvReady = frame[1] & (1 << 6);

        if ( newEpvReady != epvReady )
        {
            epvReady = newEpvReady;
            emit epvReadyChanged (epvReady);
            emit whateverChanged ();
        }

        bool newEpvReleased = !( frame[6] & (1 << 5) );

        if ( newEpvReleased != epvReleased )
        {
            epvReleased = newEpvReleased;
            emit epvReleasedChanged (epvReleased);
            emit whateverChanged ();
        }

        if ( theFirstTime )
        {
            theFirstTime = false;
            emit epvReadyChanged (epvReady);
            emit epvReleasedChanged (epvReleased);
            emit whateverChanged ();
        }
    }
}
