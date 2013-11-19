#ifndef MCO_STATE_H
#define MCO_STATE_H

#include "canblokmessage.h"

// MCO_STATE
// id: 0x050
// len: 8
// desc:0x0A08
class McoState : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    explicit McoState(QObject *parent = 0);

    bool isEpvReady () const { return epvReady; }
    bool isEpvReleased () const { return epvReleased; }
    bool isTraction () const { return traction; }
    bool isConClosed () const { return conClosed; }

signals:
    void epvReadyChanged (bool epvReady);
    void epvReleasedChanged (bool epvReleased);
    void tractionChanged (bool traction);
    void conClosedChanged (bool close);

public slots:
    bool setEpvReady (bool ready);
    bool setEpvReleased (bool released);
    bool setTraction (bool traction);
    bool setConClosed (bool closed);
    
private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    bool epvReady;
    bool epvReleased;
    bool traction;
    bool conClosed;
};

#endif // MCO_STATE_H
