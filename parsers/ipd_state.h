#ifndef IPD_STATE_H
#define IPD_STATE_H

#include "canblokmessage.h"

// IPD_STATE
// id: 0x0C4
// len: 8
// desc:0x1888
class IpdState : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    explicit IpdState(QObject *parent = 0);

    bool isInMotion () const { return inMotion; }
    int getActiveDpsNumber () const { return activeDps; }

signals:
    void inMotionChanged (bool inMotion);
    void activeDpsNumberChanged (int activeDpsNumber);

public slots:
    bool setInMotion (bool motion);
    bool setActiveDpsNumber (int activeDpsNumber);
    
private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    bool inMotion;
    int activeDps;
};

#endif // IPD_STATE_H
