#ifndef BILLCD_H
#define BILLCD_H

#include "canblokmessage.h"

// BIL_LCD_1 (BIL_LCD_2)
// id: 0x680 (0x682)
// len: 1
// desc:0xD001 (0xD041)
class BilLcd : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    BilLcd(int cabine, QObject *parent);

    bool getLcdBacklight () const { return lcdBacklight; }
    bool getKeyboardBacklight () const { return keyboardBacklight; }
    bool getAutoControl () const { return autoControl; }
    int getLight () const { return light; }

signals:
    void lcdBacklightChanged (bool );
    void keyboardBacklightChanged (bool );
    void autoControlChanged (bool );
    void lightChagned (int );

public slots:
    bool setLcdBacklight (bool v);
    bool setKeyboardBacklight (bool v);
    bool setAutoControl (bool v);
    bool setLight (int v);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    bool lcdBacklight;
    bool keyboardBacklight;
    bool autoControl;
    int light;
};

#endif // BILLCD_H
