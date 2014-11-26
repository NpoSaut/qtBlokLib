#ifndef IPD_DATE_H
#define IPD_DATE_H
#include "canblokmessage.h"

#include <QDateTime>

// IPD_DATE
// id: 0x0C7
// len: 7
// desc: 0x18E7
class IpdDate : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit IpdDate(QObject *parent = 0);
    QDateTime getDateTime () const { return dateTime; }

signals:
    void dateTimeChanged (QDateTime );

public slots:
    bool setDateTime (QDateTime dt);

protected:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    QDateTime dateTime;
};

#endif // IPD_DATE_H
