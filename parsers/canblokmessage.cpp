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

// ------------------------ PeriodicalCanBlokMessage ------------------------------

PeriodicalCanBlokMessage::PeriodicalCanBlokMessage(int id, unsigned int size, QObject *parent) :
    CanBlokMessage (id, size, parent),
    fresh (false),
    checkin (false)
{
    startTimer (1000);
}

void PeriodicalCanBlokMessage::timerEvent(QTimerEvent *event)
{
    setFresh (checkin == true);
    checkin = false; // Для следующего контроля
}

void PeriodicalCanBlokMessage::processCanMessage(CanFrame canFrame)
{
    checkin = true;
    CanBlokMessage::processCanMessage (canFrame);
}

void PeriodicalCanBlokMessage::setFresh(bool f)
{
    if ( fresh != f || theFirstTime )
    {
        fresh = f;
        emit freshChanged (fresh);
    }
}

