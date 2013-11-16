#ifndef DISPLAY_STATE_H
#define DISPLAY_STATE_H

#include "canblokmessage.h"

class DisplayStateA : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit DisplayStateA(QObject *parent = 0);

    enum DriveMode
    {
        TRAIN = 0,
        SHUNTING = 1,
        WORKING = 2,
        TWIN = 3,
        ROAD = 4
    };

    void setRb (bool press) {rb = press;}
    void setRbs (bool press) {rbs = press;}
    void setVk (bool press) {vk = press;}
    void setDriveMode (DriveMode mode) {this->mode = mode;}
    void setPull (bool press) {pull = press;}
    void setOtpr (bool press) {otpr = press;}
    void setOc (bool press) {oc = press;}
    void setK20 (bool press) {k20 = press;}
    void setFreq (bool press) {freq = press;}

    bool isRbPressed () const {return rb;}
    bool isRbsPressed () const {return rbs;}
    bool isVkPressed () const {return vk;}
    DriveMode getDriveMode () const {return mode;}
    bool isPullPressed () const {return pull;}
    bool isOtprPressed () const {return otpr;}
    bool isOcPressed () const {return oc;}
    bool isK20Pressed () const {return k20;}
    bool isFreqPressed () const {return freq;}

    CanFrame encode () const;
    
signals:
    // Нужно ещё сигналы.... Блин, нет, не могу больше..
    void whateverChanged ();

private slots:
    void getCanMessage (CanFrame frame);

private:
    bool rb;
    bool rbs;
    bool vk;
    DriveMode mode;
    bool pull;
    bool otpr;
    bool oc;
    bool k20;
    bool freq;
    
};

#endif // DISPLAY_STATE_H
