#ifndef COOKIES_H
#define COOKIES_H

#include <QObject>

#include "iodrv/canframe.h"

// Класс - локальная версия некоторого значения, хранимого в МПХ под индексом index
//   Обеспечивает проверку всех входящих сообщений, при обнаружении новго значения
//   обновляет локальную копию и вызывает сигнал.
//   При запросе на изменение значение, производит попытку записи в МПХ.
class Cookie : public QObject
{
    Q_OBJECT
public:
    explicit Cookie(QObject *parent = 0);
    Cookie (int index);

    // Возвращает локальную копию значения, хранимого в МПХ
    int getValue() const;

signals:
    // Сиглнал испускается при получении из CAN
    // значения, отличающего от хранимой локальной копии
    void onChange(int newValue);

public slots:
    // Устанавливает значение
    // Вызов приводит к попытке записи в МПХ по CAN (локальное сразу значение не изменяется)
    // При получении подтверждения о записи измениятся локальное значение и выдаётся сигнал onChange()
    void setVaule(int value);

private slots:
    // В этот слот должны направляться все приходящие сообщения SYS_DATA
    void loadData(const CanFrame &frame);

private:
    // Если переданно сообщение SYS_DATA с индексом равным нашему,
    // то загружает данные и возвращает true,
    // в противном случае возвращает false
    bool loadDataWithControl(const CanFrame& frame);

    int index;
    int value;
};

class Cookies : public QObject
{
    Q_OBJECT
public:
    explicit Cookies(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // COOKIES_H
