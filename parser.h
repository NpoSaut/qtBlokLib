#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QMap>

#include "qtCanLib/ICan.h"

#include "parsers/aux_resource.h"
#include "parsers/bil_bright_level.h"
#include "parsers/console_key.h"
#include "parsers/ipd_date.h"
#include "parsers/ipd_state.h"
#include "parsers/mco_limits.h"
#include "parsers/mco_mode.h"
#include "parsers/mco_state.h"
#include "parsers/mm_alt_long.h"
#include "parsers/mm_coord.h"
#include "parsers/mm_data.h"
#include "parsers/mm_station.h"
#include "parsers/mp_state.h"
#include "parsers/mvd_dd.h"
#include "parsers/saut_state.h"
#include "parsers/sound_command.h"
#include "parsers/sys_diagnostics.h"
#include "parsers/sys_key.h"
#include "parsers/tskbm_state.h"
#include "parsers/uktol_dd1.h"
#include "parsers/vds_state.h"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(ICan *onCan, QObject *parent = 0);

    QMap<AuxResource::Descriptor, AuxResourceVersion *> auxResources;
    BilBrightLevel bilBrightnessLevel;
    ConsoleKeysState consoleKey1;
    ConsoleKeysState consoleKey2;
    IpdDate ipdDate;
    IpdState ipdState;
    McoLimits mcoLimits;
    McoMode mcoMode;
    McoState mcoState;
    MmAltLong mmAltLong;
    MmCoord mmCoord;
    MmData mmData;
    MmStation mmStation;
    MpState mpState;
    MvdDd mvdDd;
    SautState sautState;
    SoundCommand soundCommand;
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
