#include "lowlevel.h"
#include "cookies.h"

Cookie::Cookie(int index)
    : QObject(), answerWaitTimer(), index (index)
{
    QObject::connect (&canDev, SIGNAL(receiveNewMessage(CanFrame)), this, SLOT(loadData(CanFrame)));

    answerWaitTimer.setSingleShot (true);
    answerWaitTimer.setInterval (answerTimeout);
    QObject::connect (&answerWaitTimer, SIGNAL(timeout()), this, SLOT(answerTimeoutHandler()));
}

int Cookie::getValue()
{
    if ( lastUpdateTimer.elapsed () > 60000 ) // Время жизни данных - одна минута
    {
        // Обновление
        requestValue (false);
        while ( answerWaitTimer.isActive () ); // Дожидаемся обновления
        if ( !valid )
            emit onValidChange (valid); // Повторяем сигнал
    }
    return value;
}

void Cookie::requestValue(bool forceUpdate)
{
    CanFrame frame( 0x0E01, std::vector<unsigned char> (1,index) );
    canDev.transmitMessage (frame);

    this->forceUpdate = forceUpdate;
    answerWaitTimer.start ();
}

bool Cookie::isValid() const
{
    return valid;
}

void Cookie::loadData(const CanFrame &frame)
{
    loadDataWithControl (frame);
}

void Cookie::answerTimeoutHandler()
{
    applyNewValidity (false);
}

bool Cookie::loadDataWithControl(const CanFrame &frame)
{
    if ( frame.getId () == 0x6265 /*|| frame.getId () == 0x6285*/ ) // SYS_DATA
    {
        auto byte = frame.getData ();
        int id = byte[0] & 0x7F;
        if ( id == index )
        {
            lastUpdateTimer.start ();
            answerWaitTimer.stop ();

            if ( byte[0] >> 7 ) // Признак ошибка
            {
                if ( byte[4] == 1 ) // Код ошибки: устройство занято
                {
                    // Повторяем запрос
                    CanFrame frame( 0x0E01, std::vector<unsigned char> (1,index) );
                    canDev.transmitMessage (frame);

                    if ( !answerWaitTimer.isActive () ) // Не перезапускать таймер, если уже запущен
                        answerWaitTimer.start ();
                }
                else
                {
                    applyNewValidity (false);
                }
            }
            else
            {
                applyNewValue (Complex<uint32_t> (byte[4], byte[3], byte[2], byte[1]));
                applyNewValidity (true); // в случае forceUpdate форсированно передадутся данные, а не достоверность
            }

            return true;
        }
    }
return false;
}

void Cookie::applyNewValue(int newValue)
{
    if ( newValue != value || forceUpdate )
    {
        forceUpdate = false;
        value = newValue;
        emit onChange (value);
    }
}

void Cookie::applyNewValidity(bool newValid)
{
    if ( newValid != valid || forceUpdate )
    {
        forceUpdate = false;
        valid = newValid;
        emit onValidChange (valid);
    }
}
