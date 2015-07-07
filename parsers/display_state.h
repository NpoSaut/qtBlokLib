#ifndef DISPLAY_STATE_H
#define DISPLAY_STATE_H

#include "drivemode.h"
#include "canblokmessage.h"

// DISP_STATE_A
// id: 0x28F
// len: 3
// desc:0x51E3
class DisplayStateA : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    explicit DisplayStateA(QObject *parent = 0);

    bool isRbPressed () const {return rb;}
    bool isRbsPressed () const {return rbs;}
    bool isVkPressed () const {return vk;}
    DriveMode getDriveMode () const {return mode;}
    bool isPullPressed () const {return pull;}
    bool isOtprPressed () const {return otpr;}
    bool isOcPressed () const {return oc;}
    bool isK20Pressed () const {return k20;}
    bool isFreqPressed () const {return freq;}
    bool isTvkPressed () const {return tvk;}
    int getBacklightLevel () const { return backlightLevel; }
    
signals:
    void rbChanged (bool press);
    void rbsChanged (bool press);
    void vkChanged (bool press);
    void driveModeChanged (DriveMode mode);
    void pullChanged (bool press);
    void otprChanged (bool press);
    void ocChanged (bool press);
    void k20Changed (bool press);
    void freqChanged (bool press);
    void tvkChanged (bool press);
    void backlightLevelChanged (int );

public slots:
    bool setRb (bool press);
    bool setRbs (bool press);
    bool setVk (bool press);
    bool setDriveMode (DriveMode newMode);
    bool setPull (bool press);
    bool setOtpr (bool press);
    bool setOc (bool press);
    bool setK20 (bool press);
    bool setFreq (bool press);
    bool setTvk (bool press);
    bool setBacklightLevel (int v);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    bool rb;
    bool rbs;
    bool vk;
    DriveMode mode;
    bool pull;
    bool otpr;
    bool oc;
    bool k20;
    bool freq;
    bool tvk;
    int backlightLevel;
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

signals:
    void rbChanged (bool pressed);
    void rbsChanged (bool pressed);

public slots:
    bool setRb (bool pressed);
    bool setRbs (bool pressed);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    bool rb;
    bool rbs;
};

#endif // DISPLAY_STATE_H
