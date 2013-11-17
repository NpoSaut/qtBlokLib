#include "sys_key.h"

#include <QMetaType>

SysKey::SysKey(Key key, Action action, QObject *parent) :
     CanBlokMessage(parent),
     key (key),
     action (action)
{
    qRegisterMetaType<Key> ("Key");
}

CanFrame SysKey::encode() const
{
    CanFrame frame (0x0C01);
    frame[1] = (qint8 (action) << 6) | (qint8 (key) & 0x1F);
    return frame;
}

void SysKey::processCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x0C01 ) // 0x060
    {
        if ( getAction (frame) == PRESS )
            emit keyPressed ( getKey (frame) );
        else if ( getAction (frame) == RELEASE )
            emit keyReleased ( getKey (frame) );
        whateverChanged ();

        if (theFirstTime)
            theFirstTime = false;

        emit messageReceived ();
    }
}

SysKey::Action SysKey::getAction(const CanFrame &frame) const
{
    return (Action) ((frame[1] >> 6) & 0x3);
}

SysKey::Key SysKey::getKey(const CanFrame &frame) const
{
    return (Key) (frame[1] & 0x1F);
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

void SysKeysState::processCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x0C01 )
    {
        if ( getAction (frame) == PRESS )
            pressedKeys.append (getKey (frame));
        else if ( getAction (frame) == RELEASE )
            pressedKeys.removeAll (getKey (frame));
    }

    SysKey::processCanMessage (frame);
}
