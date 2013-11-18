#ifndef MCO_STATE_H
#define MCO_STATE_H

#include "canblokmessage.h"

// MCO_STATE
// id: 0x050
// len: 8
// desc:0x0A08
class McoState : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit McoState(QObject *parent = 0);

    bool isEpvReady () const { return epvReady; }
    bool isEpvReleased () const { return epvReleased; }
    bool isTraction () const { return traction; }

signals:
    void epvReadyChanged (bool epvReady);
    void epvReleasedChanged (bool epvReleased);
    void tractionChanged (bool traction);

public slots:
    bool setEpvReady (bool ready);
    bool setEpvReleased (bool released);
    bool setTraction (bool traction);
    
private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    bool epvReady;
    bool epvReleased;
    bool traction;
};

#endif // MCO_STATE_H
