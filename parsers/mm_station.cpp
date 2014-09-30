#include "mm_station.h"

#include <QTextCodec>

MmStation::MmStation(QObject *parent) :
    PeriodicalCanBlokMessage(0x604, 8, parent),
    stationName ("        ")
{
    cp1251Decoder = QTextCodec::codecForName("CP1251")->makeDecoder ();
}

MmStation::~MmStation()
{
    delete cp1251Decoder;
}

void MmStation::fillMessage(CanFrame &frame) const
{
    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    char *name = codec->fromUnicode (getStationName()).data();
    frame.setData(std::vector<unsigned char> (name, name + 8));
}

bool MmStation::setStationName(QString name)
{
    if ( stationName != name || theFirstTime )
    {
        stationName = name;
        emit stationNameChanged (name);
        return true;
    }
    return false;
}

bool MmStation::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;
    update = setStationName( cp1251Decoder->toUnicode(
                reinterpret_cast<const char *> (frame.getData().data()),8 )
                             ) || update;
    return update;
}
