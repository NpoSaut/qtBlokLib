#include "bil_bright_level.h"

BilBrightLevel::BilBrightLevel(int brightness, QObject *parent) :
    CanBlokMessage(0x297, 1, parent),
    brightness (brightness)
{
}

void BilBrightLevel::fillMessage(CanFrame &frame) const
{
    frame [1] = brightness & 0b111;
}

bool BilBrightLevel::parseSuitableMessage(const CanFrame &frame)
{
    brightness = frame[1] & 0b111;
    emit brightnessRequested(brightness);
    return true;
}
