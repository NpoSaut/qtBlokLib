#ifndef IPD_STATE_H
#define IPD_STATE_H

#include "canblokmessage.h"

// IPD_STATE
// id: 0x0C4
// len: 8
// desc:0x1888
class IpdState : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit IpdState(QObject *parent = 0);

    bool isInMotion () const { return inMotion; }

    CanFrame encode () const;
    
signals:
    void inMotionChanged (bool inMotion);
    void whateverChanged ();
    void messageReceived ();

public slots:
    void setInMotion (bool motion);
    
private slots:
    void processCanMessage (CanFrame frame);

private:
    bool inMotion;
};

#endif // IPD_STATE_H
