#include "mco_limits.h"

#include <QMetaType>

McoLimits::McoLimits(QObject *parent) :
    CanBlokMessage(parent),
    driveMode (ROAD),
    tractionShutdownCommand (false)
{
    qRegisterMetaType<DriveMode>("DriveMode");
}

CanFrame McoLimits::encode() const
{
    CanFrame frame (0x0A48);
    frame[1] = 0;
    frame[2] = getDriveMode () == ROAD ? (1 << 6) : 0;
    frame[3] = 0;
    frame[4] = 0;
    frame[5] = 0;
    frame[6] = 0;
    frame[7] = 0;
    frame[8] = (qint8 (!isTractionShutdownCommand ()) << 7)
            | ( (qint8 (getDriveMode ()) & 0x3) << 0 );
    return frame;
}

void McoLimits::setDriveMode(DriveMode dm)
{
    if ( driveMode != dm || theFirstTime )
    {
        driveMode = dm;
        emit driveModeChanged (driveMode);
        emit whateverChanged ();
    }
}

void McoLimits::setTractionShutdownCommand(bool shutdown)
{
    if ( tractionShutdownCommand != shutdown || theFirstTime )
    {
        tractionShutdownCommand = shutdown;
        emit tractionShutdownCommandChanged (tractionShutdownCommand);
        emit whateverChanged ();
    }
}

void McoLimits::processCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x0A48 ) // id: 0x052
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

        setDriveMode (newMode);
        setTractionShutdownCommand ( !(frame[8] & (1 << 7) ) );

        if ( theFirstTime )
            theFirstTime = false;

        emit messageReceived ();
    }
}

