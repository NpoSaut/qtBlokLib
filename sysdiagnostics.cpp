#include "sysdiagnostics.h"

SysDiagnostics::SysDiagnostics(QObject *parent) :
    QObject(parent)
{
    QObject::connect (&can,SIGNAL(messageReceived(CanFrame)), this, SLOT(checkRequestInCanMessage(CanFrame)));
//    QObject::connect (this, SIGNAL(versionMessageSend(CanFrame)), &canDev, SLOT(transmitMessage(CanFrame)));
}


void SysDiagnostics::checkRequestInCanMessage(CanFrame sysDiagnostics)
{
    if (sysDiagnostics.getDescriptor () == sysDiagnosticsDescriptor)
    {
        if (sysDiagnostics.getData ()[0] == moduleCode) // Запрос адресован нашему модулю
        {
            if (sysDiagnostics.getData ()[1] == 5) // Запрос номера версии
            {
                sendVersion ();
            }
        }
    }
}

void SysDiagnostics::sendVersion()
{
    std::vector<unsigned char> data =
    {
        0, // код сообщения = RES_VERSION
        version,
        subVersion,
        0,
        0 // Контрольная сумма
    };

    CanFrame auxResourceA (auxResourceBilDescriptorA, data);
//    CanFrame auxResourceB (auxResourceBilDescriptorB, data);

    can.transmitMessage (auxResourceA);
//    canDev.transmitMessage (auxResourceB);
}

SysDiagnostics monitorSysDiagnostics;
