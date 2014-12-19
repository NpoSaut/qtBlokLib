#include "mco_state.h"

#include "QMetaType"

McoState::McoState(QObject *parent) :
    PeriodicalCanBlokMessage(0x050, 8, parent),
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
            | (qint8 (!isTraction ()) << 5);
    frame[2] = 0;
    frame[3] = 0;
    frame[4] = 0;
    frame[5] = 0;
    frame[6] = (qint8 (isEpvReleased ()) << 5)
            | (quint8 (getTrafficlight ()) & 0xF);
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

bool McoState::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;
    update =   setTraction      (!(frame[1] & (1 << 5))) || update;
    update =  setEpvReady         (frame[1] & (1 << 6))  || update;
    update =  setEpvReleased      (frame[6] & (1 << 5))  || update;
    update =  setTrafficlight     (Trafficlight (frame[6] & 0xF)) || update;
    update =  setConClosed        (frame[8] & (1 << 1))  || update;
    update =  setModulesActivity (ModulesActivity::loadFromMcoState(QByteArray((const char *)frame.getData().data(), frame.getData().size())));
    return update;
}




