#ifndef VDS_STATE_H
#define VDS_STATE_H

#include "canblokmessage.h"

// VDS_STATE_A
// id: 0x2E0
// len: 2
// desc:0x5C02
class VdsState : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit VdsState(QObject *parent = 0);

    enum TransmissionDirection
    {
        NEUTRAL = 0,
        FORWARD = 1,
        BACKWARD = 2
    };

    bool isEpvKey () const {return epvKey;}
    bool isTraction () const {return traction;}
    TransmissionDirection getTransmissionDirection () const {return transmissionDirection;}
    bool isEngineWork () const {return engineWork;}
    bool isVigilanceButton () const {return vigilanceButton;}
    bool isEmergencyStop () const {return emergencyStop;}
    bool isSiren () const {return siren;}
    bool isTifon () const {return tifon;}
    bool isIronWheels () const {return ironWheels;}

    CanFrame encode () const;
    
signals:
    void epvKeyChanged (bool key);
    void tractionChanged (bool traction);
    void transmissionDirectionChanged (TransmissionDirection td);
    void engineWorkChanged (bool work);
    void vigilanceButtonChanged (bool pressed);
    void emergencyStopChanged (bool stop);
    void sirenChanged (bool beep);
    void tifonChanged (bool tifon);
    void ironWheelsChagned (bool ironWheels);
    void whateverChanged ();
    void messageReceived ();

public slots:
    void setEpvKey (bool key);
    void setTraction (bool tr);
    void setTransmissionDirection (TransmissionDirection td);
    void setEngineWork (bool ew);
    void setVigilanceButton (bool vb);
    void setEmergencyStop (bool stop);
    void setSiren (bool sn);
    void setTifon (bool tf);
    void setIronWheels (bool iw);

private slots:
    void processCanMessage (CanFrame frame);

private:
    bool epvKey;
    bool traction;
    TransmissionDirection transmissionDirection;
    bool engineWork;
    bool vigilanceButton;
    bool emergencyStop;
    bool siren;
    bool tifon;
    bool ironWheels;
};

#endif // VDS_STATE_H
