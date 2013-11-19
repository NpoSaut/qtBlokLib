#ifndef TSKBM_STATE_H
#define TSKBM_STATE_H

#include "canblokmessage.h"

// Идентификатор: 2C0
// Длина поля данных: 1
// Дескриптор: 5801
class TskbmState : public PeriodicalCanBlokMessage
{
    Q_OBJECT
public:
    explicit TskbmState(QObject *parent = 0);

    bool isOnline() const { return online; }
    bool isMachinistCheerful() const { return machinistCheerful; }
    bool isVigilanceRequest() const { return vigilanceRequest; }
    bool isPreAlarm() const { return preAlarm; }

signals:
    void onlineChanged(bool newValue);
    void machinistCheerfulChanged(bool newValue);
    void vigilanceRequestChanged(bool newValue);
    void preAlarmChanged(bool newValue);

public slots:
    bool setOnline(bool newValue);
    bool setMachinistCheerful(bool newValue);
    bool setVigilanceRequest(bool newValue);
    bool setPreAlarm(bool newValue);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    bool online;
    bool machinistCheerful;
    bool vigilanceRequest;
    bool preAlarm;
};

#endif // TSKBM_STATE_H
