#include "ipd_date.h"

IpdDate::IpdDate(QObject *parent) :
    CanBlokMessage(0x0C7, 7, parent),
    dateTime()
{
}

bool IpdDate::setDateTime(QDateTime dt)
{
    if ( dateTime != dt || theFirstTime )
    {
        dateTime = dt;
        emit dateTimeChanged(dateTime);
        return true;
    }
    return false;
}

void IpdDate::fillMessage(CanFrame &frame) const
{
    frame[1] = (dateTime.date().year() >> 8) & 0xFF;
    frame[2] = dateTime.date().year() & 0xFF;
    frame[3] = dateTime.date().month();
    frame[4] = dateTime.date().day();

    frame[5] = dateTime.time().hour();
    frame[6] = dateTime.time().minute();
    frame[7] = dateTime.time().second();
}

bool IpdDate::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;

    int year = ( ((int)(frame[1])) << 8 ) + (int)(frame[2]);
    int month = (int) frame[3];
    int day = (int) frame[4];

    int hours = (int) frame[5];
    int minutes = (int) frame[6];
    int seconds = (int) frame[7];

    QDateTime dt (QDate(year, month, day), QTime (hours, minutes, seconds), Qt::LocalTime);
    update = setDateTime(dt) || update;

    return update;
}
