#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QMap>

#include "qtCanLib/can.h"

#include "parsers/aux_resource.h"
#include "parsers/console_key.h"
#include "parsers/ipd_state.h"
#include "parsers/mco_limits.h"
#include "parsers/mco_mode.h"
#include "parsers/mco_state.h"
#include "parsers/mm_alt_long.h"
#include "parsers/mm_coord.h"
#include "parsers/mp_state.h"
#include "parsers/mvd_dd.h"
#include "parsers/sys_diagnostics.h"
#include "parsers/sys_key.h"
#include "parsers/tskbm_state.h"
#include "parsers/uktol_dd1.h"
#include "parsers/vds_state.h"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(Can *onCan, QObject *parent = 0);

    QMap<AuxResource::Descriptor, AuxResourceVersion *> auxResources;
    ConsoleKeysState consoleKey1;
    ConsoleKeysState consoleKey2;
    IpdState ipdState;
    McoLimits mcoLimits;
    McoMode mcoMode;
    McoState mcoState;
    MmAltLong mmAltLong;
    MmCoord mmCoord;
    MpState mpState;
    MvdDd mvdDd;
    SysDiagnostics sysDiagnostics;
    SysKeysState sysKey;
    TskbmState tskbmState;
    UktolDd1 uktolDd1;
    VdsState vdsState;
    
signals:
    void whateverChagned ();
    
private slots:
    void getChildChagnedSignal ();
    
};

#endif // PARSER_H
