#ifndef MCO_STATE_H
#define MCO_STATE_H

#include "canblokmessage.h"
#include "trafficlight.h"

// MCO_STATE
// id: 0x050
// len: 8
// desc:0x0A08
class McoState : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    explicit McoState(QObject *parent = 0);

    bool isTraction () const { return traction; }
    bool isEpvReady () const { return epvReady; }
    bool isEpvReleased () const { return epvReleased; }
    Trafficlight getTrafficlight () const { return trafficlight; }
    bool isConClosed () const { return conClosed; }

signals:
    void tractionChanged (bool traction);
    void epvReadyChanged (bool epvReady);
    void epvReleasedChanged (bool epvReleased);
    void trafficlightChanged (Trafficlight light);
    void conClosedChanged (bool close);

public slots:
    bool setTraction (bool traction);
    bool setEpvReady (bool ready);
    bool setEpvReleased (bool released);
    bool setTrafficlight (Trafficlight light);
    bool setConClosed (bool closed);
    
private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    bool traction;
    bool epvReady;
    bool epvReleased;
    Trafficlight trafficlight;
    bool conClosed;
};

#endif // MCO_STATE_H
