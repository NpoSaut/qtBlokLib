#ifndef CONSOLE_KEY_H
#define CONSOLE_KEY_H

#include <QList>

#include "canblokmessage.h"

// CONSOLE_KEY
// id: 0x062 [0x063]
// len: 1
// desc: 0x0C41 [0x0C61]
class ConsoleKey : public CanBlokMessage
{
    Q_OBJECT
public:
    enum ConsKey
    {
        P   = 0x00,
        K   = 0x01,
        L   = 0x02,
        OTM = 0x03,
        P1  = 0x04,
        P2  = 0x05,
        P3  = 0x06,
        PLUS= 0x08,
        F1  = 0x09,
        F2  = 0x0A,
        F3  = 0x0B,
        CZER= 0x10,
        VK  = 0x11,
        RMP = 0x12,
        F   = 0x13,
        RB  = 0x14,
        RBS = 0x15,
        RBP = 0x16,
        MIN = 0x18,
        F4  = 0x19,
        F5  = 0x1A,
        F6  = 0x1B,
        ENTER=0x20,
        POD = 0x21,
        OTPR= 0x22,
        OC  = 0x23,
        START=0x24,
        LEFT= 0x25,
        RIGTH=0x26,
        K20 = 0x28,
        LGHT= 0x29,
        F7  = 0x2A,
        F8  = 0x2B,
        F9  = 0x30,
        F0  = 0x31,
        BKSP= 0x32,
        ERR = 0x38,
        NONE= 0x39
    };
    enum Action
    {
        RESERV0 = 0,
        PRESS = 1,
        RELEASE = 2,
        RESERV3 = 3
    };

    explicit ConsoleKey(ConsKey key = NONE, Action action = RESERV0, int cabine = 0, QObject *parent = 0);

signals:
    void keyPressed (ConsoleKey::ConsKey key);
    void keyReleased (ConsoleKey::ConsKey key);

protected:
    Action decodeAction (const CanFrame &frame) const;
    ConsKey decodeKey (const CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

private:
    void fillMessage (CanFrame &frame) const;

    ConsKey key;
    Action action;
};

class ConsoleKeysState : public ConsoleKey
{
    Q_OBJECT
public:
    explicit ConsoleKeysState(int cabine = 0, QObject *parent = 0);

    bool isKeyPressed (ConsKey key) const;

private:
    bool parseSuitableMessage (const CanFrame &frame);

    QList<ConsKey> pressedKeys;
};

#endif // CONSOLE_KEY_H
