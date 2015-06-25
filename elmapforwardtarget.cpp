#include "elmapforwardtarget.h"

#include <QTextCodec>

#include "cDoodahLib/lowlevel.h"

QTextDecoder *cp1251Decoder = QTextCodec::codecForName("CP1251")->makeDecoder ();

ElmapForwardTarget::ElmapForwardTarget(ICan *onCan, QObject *parent) :
    QObject(parent),
    name (), distance (-1), kind (-1),
    can(onCan)
{
    QObject::connect (can, SIGNAL(received(CanFrame)), this, SLOT(getNameFromMmSignal(CanFrame)));
    QObject::connect (can, SIGNAL(received(CanFrame)), this, SLOT(getDistanceFromMcoState(CanFrame)));
    QObject::connect (can, SIGNAL(received(CanFrame)), this, SLOT(getKindFromMcoLimits(CanFrame)));
}

void ElmapForwardTarget::getNameFromMmSignal(CanFrame mmSignal)
{
    if ( mmSignal.getDescriptor () == 0xC068 ) // id: 0x603
    {
        QString newName = cp1251Decoder->toUnicode(
                    reinterpret_cast<const char *> (mmSignal.getData().data()),
                    8 ).trimmed();

        if ( newName != name )
        {
            name = newName;
            emit nameChanged (name);
        }
    }
}

void ElmapForwardTarget::getDistanceFromMcoState (CanFrame mcoState)
{
    if ( mcoState.getDescriptor () == 0x0A08 ) // id: 0x050 MCO_STATE
    {
        signed char high = unsigned(mcoState[4] & 0x1F);
        qint16 newDistance = (quint16( high & (1 << 4) ? high | 0xF0 : high ) << 8) | mcoState[5];

        if ( newDistance != distance )
        {
            distance = newDistance;
            emit distanceChanged (distance);
        }
    }
}

void ElmapForwardTarget::getKindFromMcoLimits (CanFrame mcoLimits)
{
    if ( mcoLimits.getDescriptor () == 0x0A48 ) // id: 0x052 MCO_LIMITS
    {
        int newKind = mcoLimits[4] >> 4;

        if ( newKind != kind )
        {
            kind = newKind;
            emit kindChanged (kind);
        }
    }

}

//ElmapForwardTarget elmapForwardTarget;
