#include "mco_limits.h"

#include <QMetaType>

McoLimits::McoLimits(QObject *parent) :
    CanBlokMessage(parent),
    driveMode (ROAD),
    tractionShutdown (false)
{
       qRegisterMetaType<DriveMode>("DriveMode");
}

void McoLimits::getCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x0A48 ) // id: 0x052
    {
        DriveMode newMode;
        if ( frame[2] & (1 << 6) )
            newMode = ROAD;
        else if ( (frame[8] & 0b11) == 0 )
            newMode = TRAIN;
        else if ( (frame[8] & 0b11) == 1 )
            newMode = SHUNTING;
        else if ( (frame[8] & 0b11) == 2 )
            newMode = WORKING;
        else if ( (frame[8] & 0b11) == 3 )
            newMode = DOUBLE_TRACTION;

        if ( newMode != driveMode )
        {
            driveMode = newMode;
            emit driveModeChanged (driveMode);
            emit whateverChanged ();
        }

        bool newTractionShutdown = !( frame[8] & (1 << 7) );

        if ( newTractionShutdown != tractionShutdown )
        {
            tractionShutdown = newTractionShutdown;
            emit tractionShutdownChandeg (tractionShutdown);
            emit whateverChanged ();
        }

        if ( theFirstTime )
        {
            emit driveModeChanged (driveMode);
            emit tractionShutdownChandeg (tractionShutdown);
            emit whateverChanged ();
        }
    }
}
