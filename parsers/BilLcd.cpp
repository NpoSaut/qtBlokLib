#include "BilLcd.h"

BilLcd::BilLcd(int cabine, QObject *parent)
    : PeriodicalCanBlokMessage ( cabine == 2 ? 0x682 : 0x680, 1, parent),
      lcdBacklight (false),
      keyboardBacklight (false),
      autoControl (false),
      light (0)
{
}

void BilLcd::fillMessage(CanFrame &frame) const
{
    frame[1] = uint8_t (lcdBacklight) << 5 |
               uint8_t (keyboardBacklight) << 4 |
               uint8_t (autoControl) << 3 |
               uint8_t (light & 0b111);
}

bool BilLcd::setLcdBacklight(bool v)
{
    if ( lcdBacklight == v || theFirstTime )
    {
        lcdBacklight = v;
        emit lcdBacklightChanged (v);
        return true;
    }
    return false;
}

bool BilLcd::setKeyboardBacklight(bool v)
{
    if ( keyboardBacklight == v || theFirstTime )
    {
        keyboardBacklight = v;
        emit keyboardBacklightChanged (v);
        return true;
    }
    return false;
}

bool BilLcd::setAutoControl(bool v)
{
    if ( autoControl == v || theFirstTime )
    {
        autoControl = v;
        emit autoControlChanged (v);
        return true;
    }
    return false;
}

bool BilLcd::setLight(int v)
{
    if ( light == v || theFirstTime )
    {
        light = v;
        emit lightChanged (v);
        return true;
    }
    return false;
}

bool BilLcd::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;
    update =  setLcdBacklight     (frame[1] & (1 << 5))   || update;
    update =  setKeyboardBacklight(frame[1] & (1 << 4))   || update;
    update =  setAutoControl      (frame[1] & (1 << 3))   || update;
    update =  setLight            (frame[1] & 0b111)      || update;
    return update;
}
