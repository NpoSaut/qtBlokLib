#include "cDoodahLib/lowlevel.h"
#include "cookies.h"

Cookie::Cookie(Can *onCan, int index, QObject *parent)
    : QObject(parent),
      can(onCan),
      answerWaitTimer(), index (index), valid (false), forceUpdate (false), activity(NO_ACTION), attempt (0)
{
    QObject::connect (can, SIGNAL(messageReceived(CanFrame)), this, SLOT(loadData(CanFrame)));

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
        while ( activity != NO_ACTION ); // Дожидаемся обновления
        if ( !valid )
            emit onValidChange (valid); // Повторяем сигнал
    }
    return value;
}

void Cookie::requestValue(bool forceUpdate)
{
    this->forceUpdate = forceUpdate;
    startActivity (READ_ACTION);
    requestValueRequestSend ();
}

void Cookie::setVaule(int value)
{
    this->forceUpdate = true;
    writeValue = value;
    startActivity (WRITE_ACTION);
    writeValueRequestSend ();
}

bool Cookie::isValid() const
{
    return valid;
}

void Cookie::loadData(CanFrame frame)
{
    loadDataWithControl (frame);
}

void Cookie::answerTimeoutHandler()
{
    if ( attempt < maxAttempts )
    {
        attempt ++;
        if (activity == WRITE_ACTION)
            writeValueRequestSend ();
        if (activity == READ_ACTION)
            requestValueRequestSend ();

        answerWaitTimer.start ();
    }
    else
    {
        stopActivity ();
        applyNewValidity (false);
    }
}

bool Cookie::loadDataWithControl(const CanFrame &frame)
{
    if ( frame.getDescriptor () == 0x6265 /*|| frame.getId () == 0x6285*/ ) // SYS_DATA id: 0x313
    {
        auto byte = frame.getData ();
        int id = byte[0] & 0x7F;
        if ( id == index )
        {
            lastUpdateTimer.start ();
            ActivityKind lastActivity = activity;
            stopActivity ();

            if ( byte[0] >> 7 ) // Признак ошибка
            {
                if ( byte[1] == 1 ) // Код ошибки: устройство занято
                {
                    startActivity (lastActivity);
                    if (activity == WRITE_ACTION)
                        writeValueRequestSend ();
                    if (activity == READ_ACTION)
                        requestValueRequestSend ();
                }
                else
                {
                    stopActivity ();
                    applyNewValidity (false);
                }
            }
            else
            {
                //applyNewValue (Complex<uint32_t> (byte[4], byte[3], byte[2], byte[1]));
                applyNewValue ( ((unsigned int) (byte[1]) << 3*8) +
                                ((unsigned int) (byte[2]) << 2*8) +
                                ((unsigned int) (byte[3]) << 1*8) +
                                ((unsigned int) (byte[4]) << 0*8) );
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

void Cookie::writeValueRequestSend()
{
    std::vector<unsigned char> data(5);
    data[0] = index;

    Complex<int32_t> valueByte = writeValue;
    data[1] = valueByte[3];
    data[2] = valueByte[2];
    data[3] = valueByte[1];
    data[4] = valueByte[0];

    CanFrame frame( 0x6205, data ); // INPUT_DATA id: 0x310
    can->transmitMessage (frame);
}

void Cookie::requestValueRequestSend()
{
    CanFrame frame( 0x0E01, std::vector<unsigned char> (1,index) ); // SYS_DATA_QUERY id: 0x070
    can->transmitMessage (frame);
}

void Cookie::startActivity(ActivityKind kind)
{
    attempt = 1;
    activity = kind;
    answerWaitTimer.start ();
}

void Cookie::stopActivity()
{
    answerWaitTimer.stop ();
    activity = NO_ACTION;
    attempt = 0;
}

// --------- Cookies ---------

Cookies::Cookies(Can *onCan, QObject *parent)
    : QObject(parent),
      can(onCan),
      trackNumbetNotSaved (onCan, 1),
      machinistNumber (onCan, 2),
      trainNumber (onCan, 3),
      categoryTrain (onCan, 4),
      lengthInWheels (onCan, 5),
      lengthInWagons (onCan, 6),
      locomotiveNumber (onCan, 7),
      mass (onCan, 8),
      startOrdinate (onCan, 9),
      timeshift (onCan, 10),
      locomotiveKind (onCan, 11),
      speedLimitWhite (onCan, 12),
      speedLimitRedYellow (onCan, 13),
      dozorLength (onCan, 14),
      diameter1 (onCan, 15),
      diameter2 (onCan, 16),
      dpsDentos (onCan, 17),
      configuration (onCan, 18),
      speedLimitGreen (onCan, 19),
      ordinateIncreaseDirection (onCan, 20),
      milage (onCan, 21),
      klubVersion (onCan, 22),
      trackNumberInMph (onCan, 23),
      vpdPrivate (onCan, 24),
      bilBrightnes (onCan, 25),
      sns1CabinPosition (onCan, 26),
      sns2CabinPosition (onCan, 27),
      designSpeed (onCan, 106),
      // --- Запись по MCO_DATA ---
      outOfConfUfir (onCan, 28),
      outOfConfTskbm (onCan, 29),
      outOfConfSaut (onCan, 30),
      outOfConfBil (onCan, 31),
      outOfConfIpd (onCan, 32),
      outOfConfBvu (onCan, 33),
      outOfConfMm (onCan, 34),
      outOfConfEc (onCan, 35),
      // --- Запись по BKSI_DATA ---
      errorCasset (onCan, 36),
      errorDps1 (onCan, 37),
      errorDps2 (onCan, 38),
      errorSns (onCan, 39),
      errorEpk (onCan, 40),
      errorKon (onCan, 41),
      errorEpv (onCan, 42),
      errorEpt (onCan, 43),
      errorPkm1Tv (onCan, 44),
      errorPkm1Ov (onCan, 45),
      errorPkm2Tv (onCan, 46),
      errorPkm2Ov (onCan, 47),
      timeZone (onCan, 111)
{
}


void CookiePookie::setVaule(int value)
{
    Cookie::setVaule (value);

    std::vector<unsigned char> data(5);
    data[0] = index;

    Complex<int32_t> valueByte = writeValue;
    data[1] = valueByte[3];
    data[2] = valueByte[2];
    data[3] = valueByte[1];
    data[4] = valueByte[0];

    CanFrame frame( 0x6265, data ); // SYS_DATA id
    can->transmitMessage (frame);
}


CookiePookie::CookiePookie(Can *onCan, int index, QObject *parent)
    : Cookie (onCan, index, parent)
{
}
