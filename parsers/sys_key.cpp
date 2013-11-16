#include "sys_key.h"

SysKey::SysKey(Key key, Action action, QObject *parent) :
     CanBlokMessage(parent),
     key (key),
     action (action)
{
}

void SysKey::getCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x0C01 ) // 0x060
    {
        if ( getAction (frame) == PRESS )
            emit keyPressed ( getKey (frame) );
        else if ( getAction (frame) == RELEASE )
            emit keyReleased ( getKey (frame) );
        whateverChanged ();
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

void SysKeysState::getCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x0C01 )
    {
        if ( getAction (frame) == PRESS )
            pressedKeys.append (getKey (frame));
        else if ( getAction (frame) == RELEASE )
            pressedKeys.removeAll (getKey (frame));
    }

    SysKey::getCanMessage (frame);
}
