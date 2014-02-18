#include "mco_mode.h"

#include "QMetaType"

McoMode::McoMode(QObject *parent) :
    PeriodicalCanBlokMessage (0x040, 3, parent),
    driveMode (TRAIN),
    autolockMode (AB),
    manyOnesSystem (false),
    sautDiagnostics (false),
    autolockSpeed (0),
    mobileBlockzone (false)
{
    qRegisterMetaType<DriveMode>("DriveMode");
    qRegisterMetaType<AutolockMode>("AutolockMode");
}

void McoMode::fillMessage(CanFrame &frame) const
{
    frame[1] = ( (qint8 (getDriveMode ())    & 0x3) << 4 )
            |  ( (qint8 (getAutolockMode ()) & 0x3) << 2 )
            |  (  qint8 (isManyOnesSystem ())       << 1 )
            |  (  qint8 (isSautDiagnostics ())      << 0 );
    frame[2] = quint8 (autolockSpeed);
    frame[3] = qint8 (isMobileBlockzone ()) << 7;
}

bool McoMode::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;
    update = setDriveMode    ( DriveMode   ((frame[1] >> 4) & 0x3) ) || update;
    update = setAutolockMode ( AutolockMode((frame[1] >> 2) & 0x3) ) || update;
    update = setManyOnesSystem  ( frame[1] & (1 << 1) ) || update;
    update = setSautDiagnostics ( frame[1] & (1 << 0) ) || update;
    update = setAutolockSpeed ( quint8 (frame[2]) )     || update;
    update = setMobileBlockzone ( frame[3] & (1 << 7) ) || update;
    return update;
}

bool McoMode::setDriveMode(DriveMode dm)
{
    if ( driveMode != dm || theFirstTime )
    {
        driveMode = dm;
        emit driveModeChanged (driveMode);
        return true;
    }
    return false;
}

bool McoMode::setAutolockMode(AutolockMode am)
{
    if ( autolockMode != am || theFirstTime )
    {
        autolockMode = am;
        emit autolockModeChanged (autolockMode);
        return true;
    }
    return false;
}

bool McoMode::setManyOnesSystem(bool flag)
{
    if ( manyOnesSystem != flag || theFirstTime )
    {
        manyOnesSystem = flag;
        emit manyOnesSystemChanged (manyOnesSystem);
        return true;
    }
    return false;
}

bool McoMode::setSautDiagnostics(bool flag)
{
    if ( sautDiagnostics != flag || theFirstTime )
    {
        sautDiagnostics = flag;
        emit sautDiagnosticsChanged (sautDiagnostics);
        return true;
    }
    return false;
}

bool McoMode::setAutolockSpeed(int speed)
{
    if ( autolockSpeed != speed || theFirstTime )
    {
        autolockSpeed = speed;
        emit autolockSpeedChanged (autolockSpeed);
        return true;
    }
    return false;
}

bool McoMode::setMobileBlockzone(bool flag)
{
    if ( mobileBlockzone != flag || theFirstTime )
    {
        mobileBlockzone = flag;
        emit mobileBlockzoneChanged (mobileBlockzone);
        return true;
    }
    return false;
}



