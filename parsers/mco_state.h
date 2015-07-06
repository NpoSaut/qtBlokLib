#ifndef MCO_STATE_H
#define MCO_STATE_H

#include "canblokmessage.h"
#include "trafficlight.h"
#include "../modulesactivity.h"

// MCO_STATE
// id: 0x050
// len: 8
// desc:0x0A08
class McoState : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    explicit McoState(QObject *parent = 0);

    int getActiveHalfset () const { return activeHalfset; }
    bool isTraction () const { return traction; }
    bool isEpvReady () const { return epvReady; }
    bool isEpvReleased () const { return epvReleased; }
    Trafficlight getTrafficlight () const { return trafficlight; }
    bool isConClosed () const { return conClosed; }
    ModulesActivity getModulesActivity () const { return modulesActivity; }
    int getSpeedRestriction () const { return speedRestriction; }
    int getTargetSpeed () const { return targetSpeed; }

signals:
    void activeHalfsetChanged (int halfsetNumber);
    void tractionChanged (bool traction);
    void epvReadyChanged (bool epvReady);
    void epvReleasedChanged (bool epvReleased);
    void trafficlightChanged (Trafficlight light);
    void conClosedChanged (bool close);
    void modulesActivityChanged (ModulesActivity modulesActivity);
    void speedRestrictionChanged (int speedRestriction);
    void targetSpeedChanged (int targetSpeed);

public slots:
    bool setActiveHalfset (int halfsetNumber);
    bool setTraction (bool traction);
    bool setEpvReady (bool ready);
    bool setEpvReleased (bool released);
    bool setTrafficlight (Trafficlight light);
    bool setConClosed (bool closed);
    bool setModulesActivity (ModulesActivity ma);
    bool setSpeedRestriction (int sr);
    bool setTargetSpeed (int ts);
    
private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    int activeHalfset;
    bool traction;
    bool epvReady;
    bool epvReleased;
    Trafficlight trafficlight;
    bool conClosed;
    ModulesActivity modulesActivity;
    int speedRestriction;
    int targetSpeed;
};

#endif // MCO_STATE_H
