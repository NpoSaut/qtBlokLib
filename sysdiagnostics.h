#ifndef SYSDIAGNOSTICS_H
#define SYSDIAGNOSTICS_H

#include <QObject>
#include <qtCanLib/can.h>

// Отправляет диагностическую информацию (номер версии, ...)
class SysDiagnostics : public QObject
{
    Q_OBJECT
public:
    explicit SysDiagnostics(QObject *parent = 0);
    
//signals:
//    void versionMessageSend (CanFrame auxResource);

private slots:
    void checkRequestInCanMessage (CanFrame sysDiagnostics);

private:
    constexpr static int sysDiagnosticsDescriptor = 0x8D07; // id: 0x468
    constexpr static int auxResourceBilDescriptorA = 0xD005; // id: 680
//    constexpr static int auxResourceBilDescriptorB = 0xD205; // id: 690

    constexpr static int moduleCode = 7; // Код модуля БИЛ-УМВ

    constexpr static unsigned int version = 100;
    constexpr static unsigned int subVersion = 2;

    void sendVersion ();
};

#endif // SYSDIAGNOSTICS_H
