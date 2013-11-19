#include "mco_state.h"

McoState::McoState(QObject *parent) :
    PeriodicalCanBlokMessage(0x050, 8, parent),
    epvReady (false),
    epvReleased (false),
    traction (false),
    conClosed (true)
{
}

void McoState::fillMessage(CanFrame &frame) const
{
    frame[1] = (qint8 (isEpvReady ()) << 6)
            | (qint8 (isTraction ()) << 5);
    frame[2] = 0;
    frame[3] = 0;
    frame[4] = 0;
    frame[5] = 0;
    frame[6] = (qint8 (isEpvReleased ()) << 5);
    frame[7] = 0;
    frame[8] = (quint8 (isConClosed ()) << 1);
}

bool McoState::setEpvReady(bool ready)
{
    if ( epvReady != ready || theFirstTime )
    {
        epvReady = ready;
        emit epvReadyChanged (epvReady);
        return true;
    }
    return false;
}

bool McoState::setEpvReleased(bool released)
{
    if ( epvReleased != released || theFirstTime )
    {
        epvReleased = released;
        emit epvReleasedChanged (epvReleased);
        return true;
    }
    return false;
}

bool McoState::setTraction(bool tr)
{
    if ( traction != tr || theFirstTime )
    {
        traction = tr;
        emit tractionChanged (traction);
        return true;
    }
    return false;
}

bool McoState::setConClosed(bool closed)
{
    if ( conClosed != closed || theFirstTime )
    {
        conClosed = closed;
        emit conClosedChanged (conClosed);
        return true;
    }
    return false;
}

bool McoState::parseSuitableMessage(const CanFrame &frame)
{
    return
        setEpvReady         (frame[1] & (1 << 6))
     || setEpvReleased      (frame[6] & (1 << 5))
     || setTraction         (frame[1] & (1 << 5))
     || setConClosed        (frame[8] & (1 << 1));
}




