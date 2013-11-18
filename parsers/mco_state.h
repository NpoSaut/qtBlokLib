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
    bool isConClosed () const { return conClosed; }

    CanFrame encode () const;
    
signals:
    void epvReadyChanged (bool epvReady);
    void epvReleasedChanged (bool epvReleased);
    void tractionChanged (bool traction);
    void conClosedChanged (bool close);
    void whateverChanged ();
    void messageReceived ();

public slots:
    void setEpvReady (bool ready);
    void setEpvReleased (bool released);
    void setTraction (bool traction);
    void setConClosed (bool closed);
    
private slots:
    void processCanMessage (CanFrame frame);

private:
    bool epvReady;
    bool epvReleased;
    bool traction;
    bool conClosed;
};

#endif // MCO_STATE_H
