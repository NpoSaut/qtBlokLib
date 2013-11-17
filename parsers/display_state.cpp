#include "display_state.h"

#include <QMetaType>

DisplayStateA::DisplayStateA(QObject *parent) :
    CanBlokMessage(parent),
    rb (false), rbs (false), vk (false), mode (TRAIN), pull (false), otpr (false), oc (false), k20 (false), freq (false)
{
    qRegisterMetaType<DriveMode> ("DriveMode");
}

CanFrame DisplayStateA::encode() const
{
    CanFrame frame (0x51E3); // 0x28F
    frame[1] = 0; // Нет ошибок
    frame[2] = ( (quint8)isRbPressed ()     << 7 )
             | ( (quint8)isRbsPressed ()    << 6 )
             | ( (quint8)isVkPressed ()     << 5 )
             | ( (getDriveMode () == SHUNTING) << 4 )
             | ( (quint8)isPullPressed ()   << 3 )
             | ( (quint8)isOtprPressed ()   << 2 )
             | ( (quint8)isOcPressed ()     << 1 )
             | ( (quint8)isK20Pressed ()    << 0 );
    frame[3] = ( (quint8)isFreqPressed ()    << 7 )
             | ( (getDriveMode () == WORKING)  << 6 );
    return frame;
}

void DisplayStateA::processCanMessage(CanFrame frame)
{
    if ( frame.getDescriptor () == 0x51E3 ) // id: 0x28F
    {
        setRb       (frame[2] & (1 << 7));
        setRbs      (frame[2] & (1 << 6));
        setVk       (frame[2] & (1 << 5));
        setPull     (frame[2] & (1 << 3));
        setOtpr     (frame[2] & (1 << 2));
        setOc       (frame[2] & (1 << 1));
        setK20      (frame[2] & (1 << 0));
        setFreq     (frame[3] & (1 << 7));

        if (frame[3] & (1 << 6))
            setDriveMode (WORKING);
        else if (frame[2] & (1 << 4))
            setDriveMode (SHUNTING);
        else
            setDriveMode (TRAIN);

        if ( theFirstTime )
            theFirstTime = false;

        emit messageReceived ();
    }
}
