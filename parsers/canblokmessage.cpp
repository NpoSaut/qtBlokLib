#include "canblokmessage.h"

CanBlokMessage::CanBlokMessage(int id, unsigned int size, QObject *parent) :
    QObject(parent),
    id (id), size (size),
    theFirstTime (true)
{
}

CanFrame CanBlokMessage::encode() const
{
    CanFrame frame (id*0x20 + size);
    fillMessage (frame);
    return frame;
}

void CanBlokMessage::processCanMessage(CanFrame canFrame)
{
    if ( canFrame.getId () == id && canFrame.getData ().size () == size )
    {
        if ( parseSuitableMessage (canFrame) || theFirstTime )
            emit whateverChanged ();

        if (theFirstTime)
            theFirstTime = false;

        emit messageReceived ();
    }
}

