#ifndef TSKBM_STATE_H
#define TSKBM_STATE_H

#include "canblokmessage.h"

// Идентификатор: 2C0
// Длина поля данных: 1
// Дескриптор: 5801

class TskbmState : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit TskbmState(QObject *parent = 0);

    bool isOnline() { return online; }
    bool isMachinistCheerful() { return machinistCheerful; }
    bool isVigilanceRequest() { return vigilanceRequest; }
    bool isPreAlarm() { return preAlarm; }

signals:
    void onlineChanged(bool newValue);
    void machinistCheerfulChanged(bool newValue);
    void vigilanceRequestChanged(bool newValue);
    void preAlarmChanged(bool newValue);
    void whateverChanged();

private slots:
    void getCanMessage (CanFrame frame);

private:
    bool online;
    bool machinistCheerful;
    bool vigilanceRequest;
    bool preAlarm;

    void setOnline(bool newValue);
    void setMachinistCheerful(bool newValue);
    void setVigilanceRequest(bool newValue);
    void setPreAlarm(bool newValue);
};

#endif // TSKBM_STATE_H
