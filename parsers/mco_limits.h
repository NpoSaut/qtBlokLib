#ifndef MCO_LIMITS_H
#define MCO_LIMITS_H

#include "canblokmessage.h"

enum DriveMode
{
    TRAIN = 0,
    SHUNTING = 1,
    WORKING = 2,
    DOUBLE_TRACTION = 3,
    ROAD = 4
};

class McoLimits : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit McoLimits(QObject *parent = 0);

    DriveMode getDriveMode () const { return driveMode; }
    bool isTractionShutdown () const { return tractionShutdown; }
    
signals:
    void driveModeChanged (DriveMode mode);
    void tractionShutdownChandeg (bool tractionShutdown);
    void whateverChanged ();
    
private slots:
    void getCanMessage (CanFrame frame);

private:
    DriveMode driveMode;
    bool tractionShutdown;
    
};

#endif // MCO_LIMITS_H
