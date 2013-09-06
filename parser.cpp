#include "parser.h"

#include "qtCanLib/can.h"

Parser::Parser(QObject *parent) :
    QObject(parent),
    ipdState(),
    mcoLimits(),
    mcoState(),
    mmCoord()
{
    ipdState.connect (&can, SIGNAL(messageReceived(CanFrame)),SLOT(getCanMessage(CanFrame)));
    mcoLimits.connect (&can, SIGNAL(messageReceived(CanFrame)),SLOT(getCanMessage(CanFrame)));
    mcoState.connect (&can, SIGNAL(messageReceived(CanFrame)),SLOT(getCanMessage(CanFrame)));
    mmCoord.connect (&can, SIGNAL(messageReceived(CanFrame)),SLOT(getCanMessage(CanFrame)));

    this->connect (&ipdState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoLimits, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mmCoord, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
}

Parser blokMessages;



void Parser::getChildChagnedSignal()
{
    emit whateverChagned ();
}
