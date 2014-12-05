#ifndef SYS_KEY_H
#define SYS_KEY_H

#include <QList>

#include "canblokmessage.h"

// SYS_KEY
// id: 0x060
// len: 1
// desc: 0x0C01
class SysKey : public CanBlokMessage
{
    Q_OBJECT
public:
    enum Key
    {
        DUMMY = 0x0,
        RB = 0x13,
        RBS = 0x1B,
        VK = 0x14,
        RMP = 0x16,
        ALARM = 0x19,
        FREQ = 0x1C,
        PULL = 0x21,
        OTPR = 0x25,
        OC = 0x27,
        K20 = 0x29
    };
    enum Action
    {
        RESERV0 = 0,
        PRESS = 1,
        RELEASE = 2,
        RESERV3 = 3
    };

    explicit SysKey(Key key = DUMMY, Action action = RESERV0, QObject *parent = 0);

signals:
    void keyPressed (SysKey::Key key);
    void keyReleased (SysKey::Key key);

protected:
    Action decodeAction (const CanFrame &frame) const;
    Key decodeKey (const CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

private:
    void fillMessage (CanFrame &frame) const;

    Key key;
    Action action;
};

class SysKeysState : public SysKey
{
    Q_OBJECT
public:
    explicit SysKeysState(QObject *parent = 0);

    bool isKeyPressed (Key key) const;

private:
    bool parseSuitableMessage (const CanFrame &frame);

    QList<Key> pressedKeys;
};

#endif // SYS_KEY_H
