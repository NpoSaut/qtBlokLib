#include "parser.h"

#include "qtCanLib/can.h"

Parser::Parser(Can *onCan, QObject *parent) :
    QObject(parent),
    ipdState(),
    mcoLimits(),
    mcoState(),
    mmCoord(),
    sysKey(),
    tskbmState(),
    vdsState()
{
    ipdState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mcoLimits.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mcoState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mmCoord.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    sysKey.connect (onCan, SIGNAL(messageReceived(CanFrame)), SLOT(processCanMessage(CanFrame)));
    tskbmState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    vdsState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));

    this->connect (&ipdState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoLimits, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mmCoord, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&sysKey, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&tskbmState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&vdsState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
}

void Parser::getChildChagnedSignal()
{
    emit whateverChagned ();
}
