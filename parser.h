#ifndef PARSER_H
#define PARSER_H

#include <QObject>

#include "parsers/ipd_state.h"
#include "parsers/mco_limits.h"
#include "parsers/mco_state.h"
#include "parsers/mm_coord.h"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = 0);

    IpdState ipdState;
    McoLimits mcoLimits;
    McoState mcoState;
    MmCoord mmCoord;
    
signals:
    void whateverChagned ();
    
private slots:
    void getChildChagnedSignal ();
    
};

extern Parser blokMessages;

#endif // PARSER_H
