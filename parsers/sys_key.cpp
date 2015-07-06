#include "sys_key.h"

#include <QMetaType>

SysKey::SysKey(Key key, Action action, QObject *parent) :
     CanBlokMessage(0x060, 1, parent),
     key (key),
     action (action)
{
    qRegisterMetaType<Key> ("SysKey");
}

void SysKey::fillMessage(CanFrame &frame) const
{
    frame[1] = (qint8 (action) << 6) | (qint8 (key) & 0x3F);
}

// Внимание! Здесь логика отличается от обычной.
// Сигнал испускается на каждое нажатие (даже если не было отпускания)
bool SysKey::parseSuitableMessage(const CanFrame &frame)
{
    if ( decodeAction (frame) == PRESS )
        emit keyPressed ( decodeKey (frame) );
    else if ( decodeAction (frame) == RELEASE )
        emit keyReleased ( decodeKey (frame) );

    return true;
}

SysKey::Action SysKey::decodeAction(const CanFrame &frame) const
{
    return (Action) ((frame[1] >> 6) & 0x3);
}

SysKey::Key SysKey::decodeKey(const CanFrame &frame) const
{
    return (Key) (frame[1] & 0x3F);
}

SysKeysState::SysKeysState(QObject *parent)
    : SysKey (DUMMY, RESERV0, parent),
      pressedKeys ()
{
}

bool SysKeysState::isKeyPressed(SysKey::Key key) const
{
    return pressedKeys.indexOf (key) != -1;
}

bool SysKeysState::parseSuitableMessage(const CanFrame &frame)
{
    if ( decodeAction (frame) == PRESS )
        pressedKeys.append (decodeKey (frame));
    else if ( decodeAction (frame) == RELEASE )
        pressedKeys.removeAll (decodeKey (frame));

    return SysKey::parseSuitableMessage (frame);
}

