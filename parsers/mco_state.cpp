#include "mco_state.h"

McoState::McoState(QObject *parent) :
    CanBlokMessage(parent),
    epvReady (false),
    epvReleased (false),
    traction (false),
    conClosed (true)
{
}

CanFrame McoState::encode() const
{
    CanFrame frame (0x0A08);
    frame[1] = (qint8 (isEpvReady ()) << 6)
            | (qint8 (isTraction ()) << 5);
    frame[2] = 0;
    frame[3] = 0;
    frame[4] = 0;
    frame[5] = 0;
    frame[6] = (qint8 (isEpvReleased ()) << 5);
    frame[7] = 0;
    frame[8] = (quint8 (isConClosed ()) << 1);
    return frame;
}

void McoState::setEpvReady(bool ready)
{
    if ( epvReady != ready || theFirstTime )
    {
        epvReady = ready;
        emit epvReadyChanged (epvReady);
        emit whateverChanged ();
    }
}

void McoState::setEpvReleased(bool released)
{
    if ( epvReleased != released || theFirstTime )
    {
        epvReleased = released;
        emit epvReleasedChanged (epvReleased);
        emit whateverChanged ();
    }
}

void McoState::setTraction(bool tr)
{
    if ( traction != tr || theFirstTime )
    {
        traction = tr;
        emit tractionChanged (traction);
        emit whateverChanged ();
    }
}

void McoState::setConClosed(bool closed)
{
    if ( conClosed != closed || theFirstTime )
    {
        conClosed = closed;
        emit conClosedChanged (conClosed);
        emit whateverChanged ();
    }
}

void McoState::processCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x0A08 ) // id: 0x050
    {
        setEpvReady         (frame[1] & (1 << 6));
        setEpvReleased      (frame[6] & (1 << 5));
        setTraction         (frame[1] & (1 << 5));
        setConClosed        (frame[8] & (1 << 1));

        if ( theFirstTime )
            theFirstTime = false;

        emit messageReceived ();
    }
}

