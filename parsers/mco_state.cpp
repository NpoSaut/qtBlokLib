#include "mco_state.h"

#include "QMetaType"

McoState::McoState(QObject *parent) :
    PeriodicalCanBlokMessage(0x050, 8, parent),
    activeHalfset (-1),
    traction (false),
    epvReady (false),
    epvReleased (false),
    trafficlight (WHITE),
    conClosed (true)
{
    qRegisterMetaType<Trafficlight>("Trafficlight");
}

void McoState::fillMessage(CanFrame &frame) const
{
    frame[1] = (qint8 (isEpvReady ()) << 6)
            | (qint8 (!isTraction ()) << 5)
       | ( ((getActiveHalfset()-1)&1) << 4 );
    frame[2] = quint8(getSpeedRestriction());
    frame[3] = quint8(getTargetSpeed());
    frame[4] = quint8((getSpeedRestriction() >> 1) & 0x80)
            | quint8((getTargetSpeed() >> 2) & (1<<6));
    frame[5] = 0;
    frame[6] = (qint8 (isEpvReleased ()) << 5)
            | (quint8 (getTrafficlight ()) & 0xF);
    frame[7] = 0;
    frame[8] = (quint8 (isConClosed ()) << 1);
}

bool McoState::setActiveHalfset(int halfsetNumber)
{
    if ( activeHalfset != halfsetNumber || theFirstTime )
    {
        activeHalfset = halfsetNumber;
        emit activeHalfsetChanged (activeHalfset);
        return true;
    }
    return false;
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

bool McoState::setTrafficlight(Trafficlight light)
{
    if ( trafficlight != light || theFirstTime )
    {
        trafficlight = light;
        emit trafficlightChanged (trafficlight);
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

bool McoState::setModulesActivity(ModulesActivity ma)
{
    if ( modulesActivity != ma || theFirstTime )
    {
        modulesActivity = ma;
        emit modulesActivityChanged(modulesActivity);
        return true;
    }
    return false;
}

bool McoState::setSpeedRestriction(int sr)
{
    if ( speedRestriction != sr || theFirstTime )
    {
        speedRestriction = sr;
        emit speedRestrictionChanged(speedRestriction);
        return true;
    }
    return false;
}

bool McoState::setTargetSpeed(int ts)
{
    if ( targetSpeed != ts || theFirstTime )
    {
        targetSpeed = ts;
        emit targetSpeedChanged(targetSpeed);
        return true;
    }
    return false;
}

bool McoState::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;
    update =  setActiveHalfset  (((frame[1] & (1 << 4)) >> 4) + 1) || update;
    update =  setTraction       (!(frame[1] & (1 << 5))) || update;
    update =  setEpvReady         (frame[1] & (1 << 6))  || update;
    update =  setEpvReleased      (frame[6] & (1 << 5))  || update;
    update =  setTrafficlight     (Trafficlight (frame[6] & 0xF)) || update;
    update =  setConClosed        (frame[8] & (1 << 1))  || update;
    update =  setModulesActivity (ModulesActivity::loadFromMcoState(QByteArray((const char *)frame.getData().data(), frame.getData().size())));
    update =  setSpeedRestriction(( ((int)( frame[4] & (1<<7) )) << 1 ) + (int)(frame[2]));
    update =  setTargetSpeed      ( (int (frame[4] & (1<<6)) << 2) + int (frame[3]) );
    return update;
}




