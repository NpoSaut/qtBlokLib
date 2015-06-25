#ifndef COOKIES_H
#define COOKIES_H

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>

#include "qtCanLib/ICan.h"

// Класс - локальная версия некоторого значения, хранимого в МПХ под индексом index
//   Обеспечивает проверку всех входящих сообщений, при обнаружении новго значения
//   обновляет локальную копию и вызывает сигнал.
//   При запросе на изменение значение, производит попытку записи в МПХ.
class Cookie : public QObject
{
    Q_OBJECT
public:
    explicit Cookie (ICan *onCan, int index, QObject *parent = 0);

    // Возвращает локальную копию значения, хранимого в МПХ
    //   При необходимости обновляет локальное значение по данным МПХ,
    //   на это время функция блокируется (но не дольше, чем на 100 мсек)
    int getValue();

    // Посылает запрос данных в МПХ
    //   При получении ответа будет испущен сигнал updated() за время не превыщающее таймаут
    //   Функция не блокирующая
    //   С forceUpdate один из двух сигналов придёт даже в случае, если значение не изменилось
    void requestValue(bool forceUpdate = true);

    // Возвращает признак достоверности данных
    bool isValid() const;

    static const int answerTimeout = 200;
    static const int maxAttempts = 10; // Количество попыток записи по answerTimeout.
                                       // Каждую попытку производится повтор запроса,
                                       // даже если никакой ответ не получен.

signals:
    // Сигнал испускается при получении из CAN значения, отличающегося от хранимой локальной копии,
    // или при изменении достоверности.
    void updated(int newValue, bool valid);

public slots:
    // Устанавливает значение
    //   Вызов приводит к попытке записи в МПХ по CAN (локальное значение сразу не изменяется)
    //   При получении подтверждения о записи измениятся локальное значение и выдаётся сигнал updated()
    virtual void setVaule(int value);

private slots:
    // В этот слот должны направляться все приходящие сообщения SYS_DATA
    void loadData(CanFrame frame);

    // Вызывается по истечению таймаута на ожидания ответа на запрос
    void answerTimeoutHandler();

protected:
    ICan *can;

    // Если переданно сообщение SYS_DATA с индексом равным нашему,
    // то загружает данные и возвращает true,
    // в противном случае возвращает false
    bool loadDataWithControl(const CanFrame& frame);

    void update (int newValue, bool newValid);

    // Посылает в CAN команду на запись значения
    void writeValueRequestSend ();

    // Посылвает в CAN комаду на чтение значения
    void requestValueRequestSend ();

    enum ActivityKind
    {
        NO_ACTION = 0,
        WRITE_ACTION = 1,
        READ_ACTION = 2
    };
    void startActivity (ActivityKind kind);
    void stopActivity ();

    int index;
    int value;
    bool valid;


    volatile ActivityKind activity;
    bool forceUpdate;
    int writeValue;
    int attempt;

    QElapsedTimer lastUpdateTimer;
    QTimer answerWaitTimer;
};

// Вместо записи свойства как полагается через INPUT_DATA и обновления локального кеша по ответу
// просто посылает "сообщение с ответом об успешной записи этого свойства с указанным значением".
// Используется для передачи номера пути в ЭК через параметр 1, который игнорирует МПХ в силу
// алгоритма работы ЭК старого образца.
class CookiePookie : public Cookie
{
    Q_OBJECT

public:
    explicit CookiePookie (ICan *onCan, int index, QObject *parent = 0);

public slots:
    // Устанавливает значение
    //   Вызов приводит к попытке записи в МПХ по CAN (локальное значение сразу не изменяется)
    //   При получении подтверждения о записи измениятся локальное значение и выдаётся сигнал updated()
    void setVaule(int value);
};

class Cookies : public QObject
{
    Q_OBJECT
public:
    explicit Cookies(ICan *onCan, QObject *parent = 0);

    CookiePookie trackNumbetNotSaved;
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
    Cookie designSpeed;
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
    // --- Свойства САУТ ---
    Cookie timeZone;
    Cookie monitorKhConfiguration;

signals:
    
public slots:

private: // не для всех
    ICan *can;
    
};

#endif // COOKIES_H
