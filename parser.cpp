#include "parser.h"

#include "qtCanLib/can.h"

void MmCoord::getCanMessage (CanFrame frame)
{
    if ( frame.getDescriptor () == 0xC0A3 ) // id: 0x605 MM_COORD
    {
        int newCoord = frame[1]*256*256 + frame[2]*256 + frame[3];

        if ( newCoord != coord )
        {
            coord = newCoord;
            emit railWayCoordinateChanged (coord);
        }
    }
}

Parser::Parser(QObject *parent) :
    QObject(parent),
    mmCoord()
{
    mmCoord.connect (&can, SIGNAL(messageReceived(CanFrame)),SLOT(getCanMessage(CanFrame)));
}

Parser blokMessages;

