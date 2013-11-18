#include "mm_coord.h"

MmCoord::MmCoord(QObject *parent)
    : CanBlokMessage (0x605, 3, parent),
      railWayCoordinate (0)
{
}

void MmCoord::fillMessage(CanFrame &frame) const
{
    frame[1] = getRailWayCoordinate () / 256 / 256;
    frame[2] = getRailWayCoordinate () / 256;
    frame[3] = getRailWayCoordinate ();
}

bool MmCoord::setRailWayCoordinate(int coordiante)
{
    if (railWayCoordinate != coordiante || theFirstTime)
    {
        railWayCoordinate = coordiante;
        emit railWayCoordinateChanged (railWayCoordinate);
        return true;
    }
    return false;
}

bool MmCoord::parseSuitableMessage(const CanFrame &frame)
{
    return
        setRailWayCoordinate (quint32(frame[1])*256*256 + quint32(frame[2])*256 + quint32(frame[3]));
}
