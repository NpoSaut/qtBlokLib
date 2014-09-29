#include "parser.h"

#include "qtCanLib/can.h"

Parser::Parser(Can *onCan, QObject *parent) :
    QObject(parent),
    ipdState(),
    mcoLimits(),
    mcoMode(),
    mcoState(),
    mmCoord(),
    mmAltLong(),
    mpState(),
    mvdDd(),
    sautState(),
    sysKey(),
    tskbmState(),
    uktolDd1(),
    vdsState()
{
    ipdState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mcoLimits.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mcoMode.connect (onCan, SIGNAL(messageReceived(CanFrame)), SLOT(processCanMessage(CanFrame)));
    mcoState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mmAltLong.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mmCoord.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mpState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mvdDd.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    sautState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    sysKey.connect (onCan, SIGNAL(messageReceived(CanFrame)), SLOT(processCanMessage(CanFrame)));
    tskbmState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    uktolDd1.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));
    vdsState.connect (onCan, SIGNAL(messageReceived(CanFrame)),SLOT(processCanMessage(CanFrame)));

    this->connect (&ipdState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoLimits, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoMode, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mmAltLong, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mmCoord, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mpState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mvdDd, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&sautState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&sysKey, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&tskbmState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&uktolDd1, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&vdsState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
}

void Parser::getChildChagnedSignal()
{
    emit whateverChagned ();
}
