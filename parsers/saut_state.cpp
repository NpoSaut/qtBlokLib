#include "saut_state.h"

SautState::SautState(QObject *parent) :
    PeriodicalCanBlokMessage(0x1CF, 7, parent),
    brakeFactor (0),
    distanceToTarget (0)
{
}

void SautState::fillMessage(CanFrame &frame) const
{
    float bf = getBrakeFactor();
    if ( bf < 0 )
        bf = 0;
    if ( bf > 1 )
        bf = 1;

    frame[1] = 0;
    frame[2] = 0;
    frame[3] = quint8 (bf * 255);
    frame[4] = 0;
    frame[5] = quint8 (getDistanceToTarget() >> 8);
    frame[6] = quint8 (getDistanceToTarget());
    frame[7] = 0;
    frame[8] = 0;
}

bool SautState::setBrakeFactor(float bf)
{
    if ( brakeFactor != bf || theFirstTime )
    {
        brakeFactor = bf;
        emit brakeFactorChanged( brakeFactor );
        return true;
    }
    return false;
}

bool SautState::setDistanceToTarget(int dist)
{
    if ( distanceToTarget != dist || theFirstTime )
    {
        distanceToTarget = dist;
        emit distanceToTargetChanged( distanceToTarget );
        return true;
    }
    return false;
}

bool SautState::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;
    update =  setBrakeFactor      (float(frame[3]) / 256) || update;
    update =  setDistanceToTarget ( (quint16(frame[5]) << 8) + (frame[6]) ) || update;
    return update;
}
