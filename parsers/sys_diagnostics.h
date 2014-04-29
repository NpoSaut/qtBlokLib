#ifndef SYSDIAGNOSTICS_H
#define SYSDIAGNOSTICS_H

#include "canblokmessage.h"

// SYS_DIAGNOSTICS
// id: 0x468
// len: 7
// desc:0x8D07
class SysDiagnostics : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit SysDiagnostics(QObject *parent = 0);
    
    enum AuxModule
    {
        NONE = 0,
        CO = 1,
        MM = 2,
        MP_ALS = 3,
        IPD = 4,
        MPSU = 5,
        RC = 6,
        BIL = 7,
        SAUT = 8,
        TSKBM_K = 9,
        UKTOL = 10,
        SHLUZ = 11,
        BR_UMO = 14,
        TSKBM_P = 15,
        BS_DPS = 16,
        PTK = 17,
        EPK = 18
    };

    enum AuxOperation
    {
        NOOP = 0,
        GET_VERSION = 5
    };

    AuxModule getModule () const { return module; }
    AuxOperation getOperation () const { return operation; }

signals:
    void versionRequested (SysDiagnostics::AuxModule auxModule);

    void moduleChanged (AuxModule module);
    void operationChanged (AuxOperation operation);

public slots:
    bool setModule (AuxModule mod);
    bool setOperation (AuxOperation op);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    AuxModule module;
    AuxOperation operation;
};

#endif // SYSDIAGNOSTICS_H
