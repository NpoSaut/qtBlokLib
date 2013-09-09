#include "mm_coord.h"

MmCoord::MmCoord(QObject *parent)
    : CanBlokMessage (parent),
      railWayCoordinate (0)
{
}

void MmCoord::getCanMessage (CanFrame frame)
{
    if ( frame.getDescriptor () == 0xC0A3 ) // id: 0x605 MM_COORD
    {
        int newCoord = frame[1]*256*256 + frame[2]*256 + frame[3];

        if ( newCoord != railWayCoordinate )
        {
            railWayCoordinate = newCoord;
            emit railWayCoordinateChanged (railWayCoordinate);
            emit whateverChanged ();
        }

        if ( theFirstTime )
        {
            emit railWayCoordinateChanged (railWayCoordinate);
            emit whateverChanged ();
        }
    }
}

