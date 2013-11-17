#ifndef MCO_LIMITS_H
#define MCO_LIMITS_H

#include "drivemode.h"
#include "canblokmessage.h"

// MCO_LIMITS
// id: 0x052
// len: 8
// desc:0x0A48
class McoLimits : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit McoLimits(QObject *parent = 0);

    DriveMode getDriveMode () const { return driveMode; }
    bool isTractionShutdownCommand () const { return tractionShutdownCommand; }

    CanFrame encode () const;
    
signals:
    void driveModeChanged (DriveMode mode);
    void tractionShutdownCommandChanged (bool tractionShutdownCommand);
    void whateverChanged ();
    void messageReceived ();

public slots:
    void setDriveMode (DriveMode dm);
    void setTractionShutdownCommand (bool shutdown);
    
private slots:
    void processCanMessage (CanFrame frame);

private:
    DriveMode driveMode;
    bool tractionShutdownCommand;
    
};

#endif // MCO_LIMITS_H
