#ifndef MCO_MODE_H
#define MCO_MODE_H

#include "canblokmessage.h"
#include "drivemode.h"
#include "autolockmode.h"

// MCO_MODE
// id: 0x040
// len: 3
// desc:0x0803
class McoMode : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    explicit McoMode(QObject *parent = 0);

    DriveMode getDriveMode () const { return driveMode; }
    AutolockMode getAutolockMode () const { return autolockMode; }
    bool isManyOnesSystem () const { return manyOnesSystem; }
    bool isSautDiagnostics () const { return sautDiagnostics; }
    int getAutolockSpeed () const { return autolockSpeed; }
    bool isMobileBlockzone () const { return mobileBlockzone; }
    
signals:
    void driveModeChanged (DriveMode dm);
    void autolockModeChanged (AutolockMode am);
    void manyOnesSystemChanged (bool flag);
    void sautDiagnosticsChanged (bool flag);
    void autolockSpeedChanged (int speed);
    void mobileBlockzoneChanged (bool flag);
    
public slots:
    bool setDriveMode (DriveMode dm);
    bool setAutolockMode (AutolockMode am);
    bool setManyOnesSystem (bool flag);
    bool setSautDiagnostics (bool flag);
    bool setAutolockSpeed (int speed);
    bool setMobileBlockzone (bool flag);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    DriveMode driveMode;
    AutolockMode autolockMode;
    bool manyOnesSystem;
    bool sautDiagnostics;
    int autolockSpeed;
    bool mobileBlockzone;
};

#endif // MCO_MODE_H
