#ifndef MM_DATA_H
#define MM_DATA_H
#include "canblokmessage.h"

// MM_DATA
// id: 0x211
// len: 8
// desc: 0x4228
class MmData : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit MmData(QObject *parent = 0);

    double getSpeed () const { return speed; }
    int getMilage () const { return milage; }

signals:
    void speedChanged (double kmh);
    void milageChanged (int meters);

public slots:
    bool setSpeed (double kmh);
    bool setMilage (int meters);

protected:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    double speed;
    int milage;
};

#endif // MM_DATA_H
