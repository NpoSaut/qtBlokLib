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
    
signals:
    void epvReadyChanged (bool epvReady);
    void epvReleasedChanged (bool epvReleased);
    void whateverChanged ();
    
private slots:
    void getCanMessage (CanFrame frame);

private:
    bool epvReady;
    bool epvReleased;
};

#endif // MCO_STATE_H
