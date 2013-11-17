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

    bool isOnline() const { return online; }
    bool isMachinistCheerful() const { return machinistCheerful; }
    bool isVigilanceRequest() const { return vigilanceRequest; }
    bool isPreAlarm() const { return preAlarm; }

    CanFrame encode () const;

signals:
    void onlineChanged(bool newValue);
    void machinistCheerfulChanged(bool newValue);
    void vigilanceRequestChanged(bool newValue);
    void preAlarmChanged(bool newValue);
    void whateverChanged();
    void messageReceived();

private slots:
    void processCanMessage (CanFrame frame);

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
