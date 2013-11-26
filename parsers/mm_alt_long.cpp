#include "mm_alt_long.h"

MmAltLong::MmAltLong(QObject *parent) :
    CanBlokMessage(0x213, 8, parent),
    latitude (-1),
    longitude (-1),
    valid (false)
{
}

void MmAltLong::fillMessage(CanFrame &frame) const
{
    int flat = (int) ( getLatitude() * (double)1e8 * 3.14159265359 / (double)180 );
    int flon = ((int) ( getLongitude() * (double)1e8 * 3.14159265359 / (double)180 ));
    int flonr = valid ? ( flon & (~(1 << 31)) ) : ( flon | (1 << 31) );

    frame[1]= flat & 0xFF;
    frame[2] = (flat >> 8) & 0xFF;
    frame[3] = (flat >> 16) & 0xFF;
    frame[4] = (flat >> 24) & 0xFF;

    frame[5] = flonr & 0xFF;
    frame[6] = (flonr >> 8) & 0xFF;
    frame[7] = (flonr >> 16) & 0xFF;
    frame[8] = (flonr >> 24) & 0xFF;
}

bool MmAltLong::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;

    int lat_i =((int) frame[1]) + (((int) frame[2]) << 8) + (((int) frame[3]) << 16) + (((int) frame[4]) << 24);
    update = setLatitude ( (double)lat_i * 1e-8 * 180 / 3.14159265359 ) || update;

    int lon_i =((int) frame[5]) + (((int) frame[6]) << 8) + (((int) frame[7]) << 16) + (((int) (frame[8]) & 0x7F ) << 24);
    update = setLongitude ( (double)lon_i * 1e-8 * 180 / 3.14159265359 ) || update;

    update = setValid (frame[8] & (1 << 7)) || update;

    return update;
}

bool MmAltLong::setLatitude(double lat)
{
    if ( latitude != lat || theFirstTime )
    {
        latitude = lat;
        emit latitudeChanged (latitude);
        return true;
    }
    return false;
}

bool MmAltLong::setLongitude(double lon)
{
    if ( longitude != lon || theFirstTime )
    {
        longitude = lon;
        emit longitudeChanged (longitude);
        return true;
    }
    return false;
}

bool MmAltLong::setValid(bool isValid)
{
    if ( valid != isValid || theFirstTime )
    {
        valid = isValid;
        emit validChanged (valid);
        return true;
    }
    return false;
}


