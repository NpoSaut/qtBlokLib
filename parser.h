#ifndef PARSER_H
#define PARSER_H

#include <QObject>

#include "qtCanLib/can.h"

#include "parsers/ipd_state.h"
#include "parsers/mco_limits.h"
#include "parsers/mco_state.h"
#include "parsers/mm_coord.h"
#include "parsers/sys_key.h"
#include "parsers/tskbm_state.h"
#include "parsers/vds_state.h"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(Can *onCan, QObject *parent = 0);

    IpdState ipdState;
    McoLimits mcoLimits;
    McoState mcoState;
    MmCoord mmCoord;
    SysKeysState sysKey;
    TskbmState tskbmState;
    VdsState vdsState;
    
signals:
    void whateverChagned ();
    
private slots:
    void getChildChagnedSignal ();
    
};

#endif // PARSER_H
