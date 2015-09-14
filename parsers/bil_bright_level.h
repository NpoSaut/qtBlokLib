#ifndef BIL_BRIGHT_LEVEL_H
#define BIL_BRIGHT_LEVEL_H

#include "canblokmessage.h"

// CAN_BRIGHT_LEVEL
// id: 0x297
// len: 1
// desc: 0x52E1
class BilBrightLevel : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit BilBrightLevel(int brightness = 7, QObject *parent = 0);

signals:
    void brightnessRequested (int brightness);

protected:
    bool parseSuitableMessage (const CanFrame &frame);

private:
    void fillMessage (CanFrame &frame) const;
    int brightness;
};

#endif // BIL_BRIGHT_LEVEL_H
