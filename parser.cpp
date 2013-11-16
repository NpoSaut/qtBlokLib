#include "parser.h"

#include "qtCanLib/can.h"

Parser::Parser(Can *onCan, QObject *parent) :
    QObject(parent),
    ipdState(),
    mcoLimits(),
    mcoState(),
    mmCoord(),
    tskbmState()
{
    ipdState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(getCanMessage(CanFrame)));
    mcoLimits.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(getCanMessage(CanFrame)));
    mcoState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(getCanMessage(CanFrame)));
    mmCoord.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(getCanMessage(CanFrame)));
    tskbmState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(getCanMessage(CanFrame)));

    this->connect (&ipdState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoLimits, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mmCoord, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&tskbmState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
}

void Parser::getChildChagnedSignal()
{
    emit whateverChagned ();
}
