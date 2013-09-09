#ifndef MCO_STATE_H
#define MCO_STATE_H

#include "canblokmessage.h"

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
    void whateverChanged ();
    
private slots:
    void getCanMessage (CanFrame frame);

private:
    bool epvReady;
    bool epvReleased;
    bool traction;
};

#endif // MCO_STATE_H
