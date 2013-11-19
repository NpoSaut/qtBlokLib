#include "mco_limits.h"

#include <QMetaType>

McoLimits::McoLimits(QObject *parent) :
    PeriodicalCanBlokMessage(0x052, 8, parent),
    driveMode (TRAIN),
    tractionShutdownCommand (false)
{
    qRegisterMetaType<DriveMode>("DriveMode");
}

void McoLimits::fillMessage(CanFrame &frame) const
{
    frame[1] = 0;
    frame[2] = getDriveMode () == ROAD ? (1 << 6) : 0;
    frame[3] = 0;
    frame[4] = 0;
    frame[5] = 0;
    frame[6] = 0;
    frame[7] = 0;
    frame[8] = (qint8 (!isTractionShutdownCommand ()) << 7)
            | ( (qint8 (getDriveMode ()) & 0x3) << 0 );
}

bool McoLimits::setDriveMode(DriveMode dm)
{
    if ( driveMode != dm || theFirstTime )
    {
        driveMode = dm;
        emit driveModeChanged (driveMode);
        return true;
    }
    return false;
}

bool McoLimits::setTractionShutdownCommand(bool shutdown)
{
    if ( tractionShutdownCommand != shutdown || theFirstTime )
    {
        tractionShutdownCommand = shutdown;
        emit tractionShutdownCommandChanged (tractionShutdownCommand);
        return true;
    }
    return false;
}

bool McoLimits::parseSuitableMessage(const CanFrame &frame)
{
    DriveMode newMode;
    if ( frame[2] & (1 << 6) )
        newMode = ROAD;
    else if ( (frame[8] & 0x03) == 0 )
        newMode = TRAIN;
    else if ( (frame[8] & 0x03) == 1 )
        newMode = SHUNTING;
    else if ( (frame[8] & 0x03) == 2 )
        newMode = WORKING;
    else if ( (frame[8] & 0x03) == 3 )
        newMode = DOUBLE_TRACTION;

    return
           setDriveMode (newMode)
        || setTractionShutdownCommand ( !(frame[8] & (1 << 7) ) );
}

