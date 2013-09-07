#ifndef IPD_STATE_H
#define IPD_STATE_H

#include "canblokmessage.h"

class IpdState : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit IpdState(QObject *parent = 0);

    bool isInMotion () const { return inMotion; }
    
signals:
    void inMotionChanged (bool inMotion);
    void whateverChanged ();
    
private slots:
    void getCanMessage (CanFrame frame);

private:
    bool inMotion;
};

#endif // IPD_STATE_H
