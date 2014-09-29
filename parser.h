#ifndef PARSER_H
#define PARSER_H

#include <QObject>

#include "qtCanLib/can.h"

#include "parsers/ipd_state.h"
#include "parsers/mco_limits.h"
#include "parsers/mco_mode.h"
#include "parsers/mco_state.h"
#include "parsers/mm_alt_long.h"
#include "parsers/mm_coord.h"
#include "parsers/mp_state.h"
#include "parsers/mvd_dd.h"
#include "parsers/saut_state.h"
#include "parsers/sys_key.h"
#include "parsers/tskbm_state.h"
#include "parsers/uktol_dd1.h"
#include "parsers/vds_state.h"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(Can *onCan, QObject *parent = 0);

    IpdState ipdState;
    McoLimits mcoLimits;
    McoMode mcoMode;
    McoState mcoState;
    MmAltLong mmAltLong;
    MmCoord mmCoord;
    MpState mpState;
    MvdDd mvdDd;
    SautState sautState;
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
