#include "canblokmessage.h"

CanBlokMessage::CanBlokMessage(QObject *parent) :
    QObject(parent),
    theFirstTime (true)
{
}
