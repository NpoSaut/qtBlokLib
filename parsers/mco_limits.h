#ifndef MCO_LIMITS_H
#define MCO_LIMITS_H

#include "drivemode.h"
#include "canblokmessage.h"

// MCO_LIMITS
// id: 0x052
// len: 8
// desc:0x0A48
class McoLimits : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    explicit McoLimits(QObject *parent = 0);

    DriveMode getDriveMode () const { return driveMode; }
    bool isTractionShutdownCommand () const { return tractionShutdownCommand; }
    bool isSlipping () const { return slipping; }

signals:
    void driveModeChanged (DriveMode mode);
    void tractionShutdownCommandChanged (bool tractionShutdownCommand);
    void slippingChanged (bool s);

public slots:
    bool setDriveMode (DriveMode dm);
    bool setTractionShutdownCommand (bool shutdown);
    bool setSlipping (bool s);
    
private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    DriveMode driveMode;
    bool tractionShutdownCommand;
    bool slipping;
};

#endif // MCO_LIMITS_H
