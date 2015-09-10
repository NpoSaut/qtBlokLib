#include "console_beep.h"

ConsoleBeep::ConsoleBeep(bool longBeep, QObject *parent) :
    CanBlokMessage(0x064, 1, parent),
    longBeep (longBeep)
{
}

void ConsoleBeep::fillMessage(CanFrame &frame) const
{
    frame[1] = longBeep ? 1 : 0;
}

bool ConsoleBeep::parseSuitableMessage(const CanFrame &frame)
{
    longBeep = (frame[1] != 0);
    emit beep(longBeep);
    return true;
}

