#include "console_key.h"

#include <QMetaType>

ConsoleKey::ConsoleKey(ConsKey key, Action action, int cabine, QObject *parent) :
     CanBlokMessage(cabine == 0 ? 0x062 : 0x063, 1, parent),
     key (key),
     action (action)
{
    qRegisterMetaType<ConsKey> ("ConsKey");
}

void ConsoleKey::fillMessage(CanFrame &frame) const
{
    frame[1] = (qint8 (action) << 6) | (qint8 (key) & 0x1F);
}

// Внимание! Здесь логика отличается от обычной.
// Сигнал испускается на каждое нажатие (даже если не было отпускания)
bool ConsoleKey::parseSuitableMessage(const CanFrame &frame)
{
    if ( decodeAction (frame) == PRESS )
        emit keyPressed ( decodeKey (frame) );
    else if ( decodeAction (frame) == RELEASE )
        emit keyReleased ( decodeKey (frame) );

    return true;
}

ConsoleKey::Action ConsoleKey::decodeAction(const CanFrame &frame) const
{
    return (Action) ((frame[1] >> 6) & 0x3);
}

ConsoleKey::ConsKey ConsoleKey::decodeKey(const CanFrame &frame) const
{
    return (ConsKey) (frame[1] & 0x1F);
}

ConsoleKeysState::ConsoleKeysState(int cabine, QObject *parent)
    : ConsoleKey (NONE, RESERV0, cabine, parent),
      pressedKeys ()
{
}

bool ConsoleKeysState::isKeyPressed(ConsoleKey::ConsKey key) const
{
    return pressedKeys.indexOf (key) != -1;
}

bool ConsoleKeysState::parseSuitableMessage(const CanFrame &frame)
{
    if ( decodeAction (frame) == PRESS )
        pressedKeys.append (decodeKey (frame));
    else if ( decodeAction (frame) == RELEASE )
        pressedKeys.removeAll (decodeKey (frame));

    return ConsoleKey::parseSuitableMessage (frame);
}

