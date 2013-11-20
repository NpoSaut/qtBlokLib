#ifndef CANBLOKMESSAGE_H
#define CANBLOKMESSAGE_H

#include <QObject>

#include "qtCanLib/canframe.h"

// Абстрактный класс для кодирования/декодирования CAN-сообщений системы БЛОК
class CanBlokMessage : public QObject
{
    Q_OBJECT

public:
    // Кодирует данные объекта (возвращает CAN-сообщение)
    CanFrame encode () const;

public slots:
    // Если принятое CAN-сообщение соответсвует обрабатываемуму (совпадают дескрипторы),
    // то производит парсинг соощения и заполняет свойства объекта.
    void processCanMessage (CanFrame canFrame);

signals:
    // При изменении хотя бы одного свойства в процессе парсинга
    void whateverChanged ();

    // При поступлении в функцию prcessCanMessage сообщения с ожидаемым дескриптором
    void messageReceived ();

protected:
    // Конструктор должен вызваться наследником с нужными id и size (дескриптором)
    explicit CanBlokMessage (int id, unsigned int size, QObject *parent = 0);

    // Вызывается при encode после создания сообщения с нужным дескриптором
    virtual void fillMessage (CanFrame &frame) const = 0;

    // Вызывается при парсинге после проверки того, что сообщение имеет подходящий дескриптор
    // должен вернуть true, если изменилось хотя бы одной свойство
    virtual bool parseSuitableMessage (const CanFrame &frame) = 0;

    int id;
    unsigned int size;

    // Используется для того, чтобы при принятии первого сообщения после инициализации
    // вызвать все сигналы с изменением свойств, даже если эти свойства не поменялись отностильно дефолтных
    // !Класс-наследник, на тебя возлагается ответсвенность по проверки этой метки, когда испускаешь сигналы!
    bool theFirstTime;
};

// Для периодических сообщений осуществляет проверку их периодичности
class PeriodicalCanBlokMessage : public CanBlokMessage
{
    Q_OBJECT

public:
    // Возвращает false, если данные давно не подтверждались (CAN-сообщения не приходили)
    bool isFresh () const { return fresh; }

signals:
    // Если CAN-сообщения не приходили в течении заданного времени испускает false
    // если после отсутвия CAN-сообщений вновь пришло сообщение, испускает true
    void freshChanged (bool fresh);

protected slots:
    // Отмечает приход сообщения
    void checkinMessage ();

protected:
    // Конструктор должен вызваться наследником с нужными id и size (дескриптором)
    explicit PeriodicalCanBlokMessage (int id, unsigned int size, QObject *parent = 0);

    // Проверка периодичности прихода CAN-сообщений (fresh-контроль)
    void timerEvent (QTimerEvent *event);

    // Обновитель, отслеживающий изменения и испускающий freshChanged()
    void setFresh (bool f);

    // Флаг наличия переодического получения CAN-сообщений
    bool fresh;

    // Флаг снимается по timerEvent и выставляется по приходу сообщения в processCanMessage
    // Если по заходу в timerEvent флаг обнаруживается не выставленным, то fresh устанавливается в false
    // В противном случае fresh устанавливается в true
    bool checkin;
};

#endif // CANBLOKMESSAGE_H
