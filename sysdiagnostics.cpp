#include "sysdiagnostics.h"

SysDiagnostics::SysDiagnostics(Can *onCan, QObject *parent) :
    QObject(parent),
    can(onCan)
{
    QObject::connect (can,SIGNAL(messageReceived(CanFrame)), this, SLOT(checkRequestInCanMessage(CanFrame)));
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
    std::vector<unsigned char> data(5);
    data.push_back(0); // код сообщения = RES_VERSION
    data.push_back(version);
    data.push_back(subVersion);
    data.push_back(0);
    data.push_back(0); // Контрольная сумма

    CanFrame auxResourceA (auxResourceBilDescriptorA, data);
//    CanFrame auxResourceB (auxResourceBilDescriptorB, data);

    can->transmitMessage (auxResourceA);
//    canDev.transmitMessage (auxResourceB);
}
