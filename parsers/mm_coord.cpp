#include "mm_coord.h"

MmCoord::MmCoord(QObject *parent)
    : CanBlokMessage (parent),
      railWayCoordinate (0)
{
}

CanFrame MmCoord::encode() const
{
    CanFrame frame (0xC0A3);
    frame[1] = getRailWayCoordinate () / 256 / 256;
    frame[2] = getRailWayCoordinate () / 256;
    frame[3] = getRailWayCoordinate ();
    return frame;
}

void MmCoord::setRailWayCoordinate(int coordiante)
{
    if (railWayCoordinate != coordiante || theFirstTime)
    {
        railWayCoordinate = coordiante;
        emit railWayCoordinateChanged (railWayCoordinate);
        emit whateverChanged ();
    }
}

void MmCoord::processCanMessage (CanFrame frame)
{
    if ( frame.getDescriptor () == 0xC0A3 ) // id: 0x605 MM_COORD
    {
        setRailWayCoordinate (quint32(frame[1])*256*256 + quint32(frame[2])*256 + quint32(frame[3]));

        if ( theFirstTime )
            theFirstTime = false;

        emit messageReceived ();
    }
}
