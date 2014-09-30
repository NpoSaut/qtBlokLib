#ifndef MM_STATION_H
#define MM_STATION_H

#include <QString>
#include <QTextDecoder>

#include "drivemode.h"
#include "canblokmessage.h"

// MM_STATION
// id: 0x604
// len: 8
// desc:0xC088
class MmStation : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    explicit MmStation(QObject *parent = 0);
    ~MmStation ();

    QString getStationName () const { return stationName; }

signals:
    void stationNameChanged (QString name);

public slots:
    bool setStationName (QString name);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    QString stationName;
    QTextDecoder *cp1251Decoder;
};

#endif // MM_STATION_H
