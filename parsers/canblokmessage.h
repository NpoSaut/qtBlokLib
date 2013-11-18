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
    // Если переданное CAN-сообщение соответсвует обрабатываемуму (совпадают дескрипторы),
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

#endif // CANBLOKMESSAGE_H
