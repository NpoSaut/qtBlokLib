#ifndef DISPLAY_STATE_H
#define DISPLAY_STATE_H

#include "drivemode.h"
#include "canblokmessage.h"

// DISP_STATE_A
// id: 0x28F
// len: 3
// desc:0x51E3
class DisplayStateA : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit DisplayStateA(QObject *parent = 0);

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
    void messageReceived ();

private slots:
    void processCanMessage (CanFrame frame);

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

// DISP_STATE_B
// id: 0x29F
// len: 2
// desc:0x53E2
class DisplayStateB : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit DisplayStateB(QObject *parent = 0);

    bool isRbPressed () const { return rb; }
    bool isRbsPressed () const { return rbs; }

    CanFrame encode () const;

signals:
    void rbChanged (bool pressed);
    void rbsChanged (bool pressed);
    void whateverChanged ();
    void messageReceived ();

public slots:
    void setRb (bool pressed);
    void setRbs (bool pressed);

private slots:
    void processCanMessage (CanFrame frame);

private:
    bool rb;
    bool rbs;
};

#endif // DISPLAY_STATE_H
