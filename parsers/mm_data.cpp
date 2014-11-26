#include "mm_data.h"

MmData::MmData(QObject *parent) :
    CanBlokMessage(0x211, 8, parent),
    speed (-1),
    milage (-1)
{
}

bool MmData::setSpeed(double kmh)
{
    if ( speed != kmh || theFirstTime )
    {
        speed = kmh;
        emit dateTimeChanged(speed);
        return true;
    }
    return false;
}

bool MmData::setMilage(int meters)
{
    if ( milage != meters || theFirstTime )
    {
        milage = meters;
        emit dateTimeChanged(milage);
        return true;
    }
    return false;
}

void MmData::fillMessage(CanFrame &frame) const
{
    frame[1] = 0;
    frame[2] = int(speed) & 0xFF;
    frame[3] = 0;
    frame[4] = int(milage)/256/256;
    frame[5] = int(milage)/256;
    frame[6] = int(milage);
    frame[7] = 0;
    frame[8] = 0;
}

bool MmData::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;

    update = setSpeed(frame[2]) || update;
    update = setMilage(int(frame[4])*256*256 + int(frame[5])*256 + int(frame[6])) || update;

    return update;
}
