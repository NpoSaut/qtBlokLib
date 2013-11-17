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

    CanFrame encode () const;
    
signals:
    void epvReadyChanged (bool epvReady);
    void epvReleasedChanged (bool epvReleased);
    void tractionChanged (bool traction);
    void whateverChanged ();
    void messageReceived ();

public slots:
    void setEpvReady (bool ready);
    void setEpvReleased (bool released);
    void setTraction (bool traction);
    
private slots:
    void processCanMessage (CanFrame frame);

private:
    bool epvReady;
    bool epvReleased;
    bool traction;
};

#endif // MCO_STATE_H
