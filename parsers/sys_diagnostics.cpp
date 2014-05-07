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
