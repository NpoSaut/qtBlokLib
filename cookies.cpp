// Временно: чтобы под виндоус у Жени компилировалось
#if defined WITH_CAN

#include "cDoodahLib/lowlevel.h"
#include "cookies.h"

Cookie::Cookie(int index)
    : QObject(), answerWaitTimer(), index (index), valid (false), forceUpdate (false), activity(NO_ACTION), attempt (0)
{
    QObject::connect (&can, SIGNAL(messageReceived(CanFrame)), this, SLOT(loadData(CanFrame)));

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

void Cookie::loadData(const CanFrame &frame)
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
            answerWaitTimer.stop ();                // !!!!

            if ( byte[0] >> 7 ) // Признак ошибка
            {
                if ( byte[1] == 1 ) // Код ошибки: устройство занято
                {
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
                stopActivity ();
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
    can.transmitMessage (frame);
}

void Cookie::requestValueRequestSend()
{
    CanFrame frame( 0x0E01, std::vector<unsigned char> (1,index) ); // SYS_DATA_QUERY id: 0x070
    can.transmitMessage (frame);
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

Cookies::Cookies(QObject *parent)
    : QObject(parent),
      trackNumbetNotSaved (1),
      machinistNumber (2),
      trainNumber (3),
      categoryTrain (4),
      lengthInWheels (5),
      lengthInWagons (6),
      locomotiveNumber (7),
      mass (8),
      startOrdinate (9),
      timeshift (10),
      locomotiveKind (11),
      speedLimitWhite (12),
      speedLimitRedYellow (13),
      dozorLength (14),
      diameter1 (15),
      diameter2 (16),
      dpsDentos (17),
      configuration (18),
      speedLimitGreen (19),
      ordinateIncreaseDirection (20),
      milage (21),
      klubVersion (22),
      trackNumberInMph (23),
      vpdPrivate (24),
      bilBrightnes (25),
      sns1CabinPosition (26),
      sns2CabinPosition (27),
      // --- Запись по MCO_DATA ---
      outOfConfUfir (28),
      outOfConfTskbm (29),
      outOfConfSaut (30),
      outOfConfBil (31),
      outOfConfIpd (32),
      outOfConfBvu (33),
      outOfConfMm (34),
      outOfConfEc (35),
      // --- Запись по BKSI_DATA ---
      errorCasset (36),
      errorDps1 (37),
      errorDps2 (38),
      errorSns (39),
      errorEpk (40),
      errorKon (41),
      errorEpv (42),
      errorEpt (43),
      errorPkm1Tv (44),
      errorPkm1Ov (45),
      errorPkm2Tv (46),
      errorPkm2Ov (47)
{
}

Cookies cookies;

#endif // WITH_CAN
