#include "parser.h"

Parser::Parser(ICan *onCan, QObject *parent) :
    QObject(parent),
    consoleKey1(),
    consoleKey2(),
    ipdDate(),
    ipdState(),
    mcoLimits(),
    mcoMode(),
    mcoState(),
    mmAltLong(),
    mmCoord(),
    mmData(),
    mmStation(),
    mpState(),
    mvdDd(),
    sautState(),
    sysDiagnostics(),
    sysKey(),
    tskbmState(),
    uktolDd1(),
    vdsState()
{
    QMetaEnum auxDescriptors = AuxResource::staticMetaObject.enumerator (
                                AuxResource::staticMetaObject.indexOfEnumerator ("Descriptor"));

    for (int i = 0; i < auxDescriptors.keyCount(); i ++)
    {
        AuxResource::Descriptor descriptor = AuxResource::Descriptor(auxDescriptors.value(i));
        auxResources[descriptor] = new AuxResourceVersion (descriptor, this);
        auxResources[descriptor]->connect (onCan, SIGNAL(received(CanFrame)), SLOT(processCanMessage(CanFrame)));
        this->connect (auxResources[descriptor], SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    }

    consoleKey1.connect (onCan, SIGNAL(received(CanFrame)), SLOT(processCanMessage(CanFrame)));
    consoleKey2.connect (onCan, SIGNAL(received(CanFrame)), SLOT(processCanMessage(CanFrame)));
    ipdDate.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    ipdState.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mcoLimits.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mcoMode.connect (onCan, SIGNAL(received(CanFrame)), SLOT(processCanMessage(CanFrame)));
    mcoState.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mmAltLong.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mmCoord.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mmData.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mmStation.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mpState.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    mvdDd.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    sautState.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    sysDiagnostics.connect (onCan, SIGNAL(received(CanFrame)), SLOT(processCanMessage(CanFrame)));
    sysKey.connect (onCan, SIGNAL(received(CanFrame)), SLOT(processCanMessage(CanFrame)));
    tskbmState.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    uktolDd1.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));
    vdsState.connect (onCan, SIGNAL(received(CanFrame)),SLOT(processCanMessage(CanFrame)));

    this->connect (&consoleKey1, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&consoleKey1, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&ipdDate, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&ipdState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoLimits, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoMode, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mcoState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mmAltLong, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mmCoord, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
	this->connect (&mmData, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mmStation, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mpState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&mvdDd, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&sautState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&sysDiagnostics, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&sysKey, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&tskbmState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&uktolDd1, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
    this->connect (&vdsState, SIGNAL(whateverChanged()), SLOT(getChildChagnedSignal()));
}

void Parser::getChildChagnedSignal()
{
    emit whateverChagned ();
}
