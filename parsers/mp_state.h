#ifndef MP_STATE_H
#define MP_STATE_H

#include "canblokmessage.h"
#include "trafficlight.h"

// MP_STATE_A
// id: 0x08F
// len: 8
// desc:0x11E8
class MpState : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit MpState(QObject *parent = 0);

    Trafficlight getTrafficlight () const { return trafficlight; }
    bool isDeviationFromNarrow () const { return deviationFromNarrow; }
    int getRestrictionSpeed () const { return restrictionSpeed; }
    int getTargetSpeed () const { return targetSpeed; }
    KptType getKptType () const { return kptType; }
    AlsnFrequency getFrequense () const { return frequency; }
    bool isAlsnActive () const { return alsnActive; }
    bool isAlsEnActive () const { return alsEnActive; }
    AlsnLight getAlsnLight () const { return alsnLight; }
    bool isAutoblockClosed () const { return autoblockClosed; }
    bool isAutoblockSemiClosed () const { return autoblockSemiClosed; }
    int getBlockzoneLength () const { return blockzoneLength; }
    
signals:
    void trafficlightChanged (Trafficlight tl);
    void deviationFromNarrowChanged (bool deviation);
    void targetSpeedChanged (int speed);
    void restrictionSpeedChanged (int speed);
    void kptTypeChanged (KptType type);
    void frequencyChanged (AlsnFrequency freq);
    void alsnActiveChanged (bool active);
    void alsEnActiveChanged (bool active);
    void alsnLightChanged (AlsnLight light);
    void autoblockClosedChanged (bool closed);
    void autoblockSemiClosedChanged (bool semiClosed);
    void blockzoneLengthChanged (int length);
    
public slots:
    bool setTrafficlight (Trafficlight tl);
    bool setDeviationFromNarrow (bool deviaton);
    bool setTargetSpeed (int speed);
    bool setRestrictionSpeed (int speed);
    bool setKptType (KptType type);
    bool setFrequency (AlsnFrequency freq);
    bool setAlsnActive (bool active);
    bool setAlsEnActive (bool active);
    bool setAlsnLight (AlsnLight light);
    bool setAutoblockClosed (bool closed);
    bool setAutoblockSemiClosed (bool semiClosed);
    bool setBlockzoneLength (int length);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);
    
    Trafficlight trafficlight;
    bool deviationFromNarrow;
    int targetSpeed;
    int restrictionSpeed;
    KptType kptType;
    AlsnFrequency frequency;
    bool alsnActive;
    bool alsEnActive;
    AlsnLight alsnLight;
    bool autoblockClosed;
    bool autoblockSemiClosed;
    int blockzoneLength;
};

#endif // MP_STATE_H
