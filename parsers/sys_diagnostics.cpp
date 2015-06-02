#include "sys_diagnostics.h"

#include <QMetaType>

SysDiagnostics::SysDiagnostics(QObject *parent) :
    CanBlokMessage(0x468, 7, parent),
    module (NONE),
    operation (NOOP)
{
    qRegisterMetaType<SysDiagnostics::AuxModule> ("SysDiagnostics::AuxModule");
    qRegisterMetaType<AuxOperation> ("AuxOperation");
}

void SysDiagnostics::fillMessage(CanFrame &frame) const
{
    frame[1] = quint8 (getModule ());
    frame[2] = quint8 (getOperation ());
    frame[3] = 0;
    frame[4] = 0;
    frame[5] = 0;
    frame[6] = 0;
    frame[7] = 0;
}

bool SysDiagnostics::setModule(SysDiagnostics::AuxModule mod)
{
    if ( module != mod || theFirstTime )
    {
        module = mod;
        emit moduleChanged (module);
        return true;
    }
    return false;
}

bool SysDiagnostics::setOperation(SysDiagnostics::AuxOperation op)
{
    if ( operation != op || theFirstTime )
    {
        operation = op;
        emit operationChanged (operation);
        return true;
    }
    return false;
}

bool SysDiagnostics::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;
    update = setModule      (AuxModule      (frame[1])) || update;
    update = setOperation   (AuxOperation   (frame[2])) || update;

    if ( getOperation () == GET_VERSION )
        emit versionRequested (getModule ());

    return update;
}


MpAlsSysDiagnostics::MpAlsSysDiagnostics(QObject *parent)
    : SysDiagnostics (parent)
{
    setModule(AuxModule::MP_ALS);
}

bool MpAlsSysDiagnostics::parseSuitableMessage(const CanFrame &frame)
{
    if (AuxModule(frame[1]) == MP_ALS)
        return SysDiagnostics::parseSuitableMessage(frame);
    else
        return false;
}


AutolockModeSysDiagnostics::AutolockModeSysDiagnostics(QObject *parent)
    : MpAlsSysDiagnostics (parent)
{
    qRegisterMetaType<AutolockMode> ("AutolockMode");
}

void AutolockModeSysDiagnostics::fillMessage(CanFrame &frame) const
{
    MpAlsSysDiagnostics::fillMessage(frame);
    frame[2] = code (getAutolockMode());
    frame[3] = getSpeedRestriction();
    frame[4] = getSpeedRestriction() >> 8;
}

bool AutolockModeSysDiagnostics::setAutolockMode(AutolockMode m)
{
    if ( mode != m || theFirstTime )
    {
        mode = m;
        emit autolockModeChanged(mode);
        return true;
    }
    return false;
}

bool AutolockModeSysDiagnostics::setRestrictionSpeed(int s)
{
    if ( speed != s || theFirstTime )
    {
        speed = s;
        emit speedRestrictionChanged(speed);
        return true;
    }
    return false;
}

bool AutolockModeSysDiagnostics::parseSuitableMessage(const CanFrame &frame)
{
    bool update = MpAlsSysDiagnostics::parseSuitableMessage(frame);

    update = setAutolockMode(decode(frame[2])) || update;
    update = setRestrictionSpeed(int(frame[4]) << 8 + frame[3]) || update;

    return update;
}

quint8 AutolockModeSysDiagnostics::code(AutolockMode mode) const
{
    switch (mode) {
    case AB:
        return 0x20;
    case PAB:
        return 0x29;
    case ZAB:
        return 0x1F;
    }
}

AutolockMode AutolockModeSysDiagnostics::decode(quint8 code) const
{
    switch (code) {
    case 0x20:
    default:
        return AB;
    case 0x29:
        return PAB;
    case 0x1F:
        return ZAB;
    }
}
