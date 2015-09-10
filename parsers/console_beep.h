#ifndef CONSOLE_BEEP_H
#define CONSOLE_BEEP_H

#include "canblokmessage.h"

// CONSOLE_BEEP
// id: 0x064 [0x065]
// len: 1
// desc: 0x0C81 [0x0CA1]
class ConsoleBeep : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit ConsoleBeep(bool longBeep = false, QObject *parent = 0);

signals:
    void beep (bool longBeep);

protected:
    bool parseSuitableMessage (const CanFrame &frame);

private:
    void fillMessage (CanFrame &frame) const;
    bool longBeep;
};

#endif // CONSOLE_BEEP_H
