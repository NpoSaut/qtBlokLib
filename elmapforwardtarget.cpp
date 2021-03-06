#include "elmapforwardtarget.h"

#include <QTextCodec>

#include "cDoodahLib/lowlevel.h"
#include "qtCanLib/can.h"

QTextDecoder *cp1251Decoder = QTextCodec::codecForName("CP1251")->makeDecoder ();

ElmapForwardTarget::ElmapForwardTarget(Can *onCan, QObject *parent) :
    QObject(parent),
    name (), distance (-1), kind (-1),
    can(onCan)
{
    QObject::connect (can, SIGNAL(messageReceived(CanFrame)), this, SLOT(getNameFromMmSignal(CanFrame)));
    QObject::connect (can, SIGNAL(messageReceived(CanFrame)), this, SLOT(getDistanceFromMcoState(CanFrame)));
    QObject::connect (can, SIGNAL(messageReceived(CanFrame)), this, SLOT(getKindFromMcoLimits(CanFrame)));
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
#ifndef WIN32
    if ( mcoState.getDescriptor () == 0x0A08 ) // id: 0x050 MCO_STATE
    {
        signed char high = unsigned(mcoState[4] & 0x1F);
        Complex<int16_t> newDistance ({mcoState[5], high & (1 << 4) ? high | 0xF0 : high });

        if ( newDistance != distance )
        {
            distance = newDistance;
            emit distanceChanged (distance);
        }
    }
#endif
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
