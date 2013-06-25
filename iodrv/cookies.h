#ifndef COOKIES_H
#define COOKIES_H

// Временно: чтобы под виндоус у Жени компилировалось
#if defined WITH_CAN

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>

#include "iodrv/can.h"

// Класс - локальная версия некоторого значения, хранимого в МПХ под индексом index
//   Обеспечивает проверку всех входящих сообщений, при обнаружении новго значения
//   обновляет локальную копию и вызывает сигнал.
//   При запросе на изменение значение, производит попытку записи в МПХ.
class Cookie : public QObject
{
    Q_OBJECT
public:
    explicit Cookie (int index);

    // Возвращает локальную копию значения, хранимого в МПХ
    //   При необходимости обновляет локальное значение по данным МПХ,
    //   на это время функция блокируется (но не дольше, чем на 100 мсек)
    int getValue();

    // Посылает запрос данных в МПХ
    //   При получении ответа будет испущен сигнал onChange или onValidChange за время не превыщающее таймаут
    //   Функция не блокирующая
    //   С forceUpdate один из двух сигналов придёт даже в случае, если значение не изменилось
    void requestValue(bool forceUpdate = true);

    // Возвращает признак достоверности данных
    bool isValid() const;

    static const int answerTimeout = 110;

signals:
    // Сиглнал испускается при получении из CAN
    // значения, отличающего от хранимой локальной копии
    void onChange(int newValue);

    // Сигнал испускается при изменениии достоверности данных
    void onValidChange(bool valid);

public slots:
    // Устанавливает значение
    //   Вызов приводит к попытке записи в МПХ по CAN (локальное значение сразу не изменяется)
    //   При получении подтверждения о записи измениятся локальное значение и выдаётся сигнал onChange()
    void setVaule(int value);

private slots:
    // В этот слот должны направляться все приходящие сообщения SYS_DATA
    void loadData(const CanFrame &frame);

    // Вызывается по истечению таймаута на ожидания ответа на запрос
    void answerTimeoutHandler();

private:
    // Если переданно сообщение SYS_DATA с индексом равным нашему,
    // то загружает данные и возвращает true,
    // в противном случае возвращает false
    bool loadDataWithControl(const CanFrame& frame);

    void applyNewValue (int newValue);
    void applyNewValidity (bool newValid);

    int index;
    int value;
    bool valid;
    bool forceUpdate;
    QElapsedTimer lastUpdateTimer;
    QTimer answerWaitTimer;
};

class Cookies : public QObject
{
    Q_OBJECT
public:
    explicit Cookies(QObject *parent = 0);

    Cookie trackNumbetNotSaved;
    Cookie machinistNumber;
    Cookie trainNumber;
    Cookie categoryTrain;
    Cookie lengthInWheels;
    Cookie lengthInWagons;
    Cookie locomotiveNumber;
    Cookie mass;
    Cookie startOrdinate;
    Cookie timeshift;
    Cookie locomotiveKind;
    Cookie speedLimitWhite;
    Cookie speedLimitRedYellow;
    Cookie dozorLength;
    Cookie diameter1;
    Cookie diameter2;
    Cookie dpsDentos;
    Cookie configuration;
    Cookie speedLimitGreen;
    Cookie ordinateIncreaseDirection;
    Cookie milage;
    Cookie klubVersion;
    Cookie trackNumberInMph;
    Cookie vpdPrivate;
    Cookie bilBrightnes;
    Cookie sns1CabinPosition;
    Cookie sns2CabinPosition;
    // --- Запись по MCO_DATA ---
    Cookie outOfConfUfir;
    Cookie outOfConfTskbm;
    Cookie outOfConfSaut;
    Cookie outOfConfBil;
    Cookie outOfConfIpd;
    Cookie outOfConfBvu;
    Cookie outOfConfMm;
    Cookie outOfConfEc;
    // --- Запсь по BKSI_DATA ---
    Cookie errorCasset;
    Cookie errorDps1;
    Cookie errorDps2;
    Cookie errorSns;
    Cookie errorEpk;
    Cookie errorKon;
    Cookie errorEpv;
    Cookie errorEpt;
    Cookie errorPkm1Tv;
    Cookie errorPkm1Ov;
    Cookie errorPkm2Tv;
    Cookie errorPkm2Ov;

signals:
    
public slots:
    
};

extern Cookies cookies;

#endif // WITH_CAN

#endif // COOKIES_H
