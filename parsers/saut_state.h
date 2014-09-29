#ifndef SAUT_STATE_H
#define SAUT_STATE_H

#include "canblokmessage.h"
#include "trafficlight.h"

// SAUT_STATE
// id: 0x1CF
// len: 7
// desc:0x39E7
class SautState : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    explicit SautState(QObject *parent = 0);

    float getBrakeFactor () const { return brakeFactor; }
    int getDistanceToTarget () const { return distanceToTarget; }

signals:
    void brakeFactorChanged (float bf);
    void distanceToTargetChanged (int dist);

public slots:
    bool setBrakeFactor (float bf);
    bool setDistanceToTarget (int dist);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    float brakeFactor;
    int distanceToTarget;
};

#endif // SAUT_STATE_H
