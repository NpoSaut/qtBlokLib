#include <QBitArray>
#include <QTextStream>

#include "modulesactivity.h"

QString ModulesActivity::toString() const
{
    QString str;

    if (isCollected ())
    {
        QTextStream s (&str);
        s << (ipd           ? "1" : "-")
          << (monitor       ? "2" : "-")
          << (ipd           ? "3" : "-")
          << (mpAls         ? "4" : "-")
          << (navigation    ? "5" : "-")
          << (electronicMap ? "6" : "-")
          << (breaksBrain   ? "7" : "-")
          << (false         ? "8" : "-")
          << (radio         ? "9" : "-")
          << (outputModule  ? "A" : "-")
          << (tskbm         ? "B" : "-")
          << (controlSystem ? "C" : "-");
    }
    else
    {
        str = "------------";
    }

    return str;
}

ModulesActivity ModulesActivity::loadFromMcoState(const QByteArray &mcoStateMessage)
{
    qDebug ("mcoState: %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x,",
            mcoStateMessage[0], mcoStateMessage[1], mcoStateMessage[2], mcoStateMessage[3], mcoStateMessage[4], mcoStateMessage[5], mcoStateMessage[6], mcoStateMessage[7]);

    ModulesActivity ma;

    ma.mpAls            = mcoStateMessage[6] & (1 << 0);
    ma.ipd              = mcoStateMessage[6] & (1 << 1);
    ma.controlSystem    = mcoStateMessage[6] & (1 << 2);
    ma.radio            = mcoStateMessage[6] & (1 << 3);
    ma.monitor          = mcoStateMessage[6] & (1 << 4);
//    ma.saut           = mcoStateMessage[6] & (1 << 5);
    ma.tskbm            = mcoStateMessage[6] & (1 << 6);
    ma.breaksBrain      = mcoStateMessage[6] & (1 << 7);

    ma.outputModule     = mcoStateMessage[7] & (1 << 4);
    ma.navigation       = mcoStateMessage[7] & (1 << 5);
    ma.electronicMap    = mcoStateMessage[7] & (1 << 7);

    ma.collected = true;

    return ma;
}

bool ModulesActivity::operator ==(const ModulesActivity &b)
{
    return ipd == b.ipd &&
            monitor == b.monitor &&
            mpAls == b.mpAls &&
            electronicMap == b.electronicMap &&
            navigation == b.navigation &&
            breaksBrain == b.breaksBrain &&
            radio == b.radio &&
            tskbm == b.tskbm &&
            controlSystem == b.controlSystem &&
            collected == b.collected;
}

//ModulesActivity::ModulesActivity(const ModulesActivity &ma)
//{
//    memcpy (this, &ma, sizeof(ModulesActivity));
//}

//ModulesActivity &ModulesActivity::operator =(const ModulesActivity& ma)
//{
//    memcpy (this, &ma, sizeof(ModulesActivity));
//}
