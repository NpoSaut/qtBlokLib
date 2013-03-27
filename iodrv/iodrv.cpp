#if defined WITH_CAN || defined WITH_SERIAL

#include "iodrv.h"
#include <QDate>
#include <QtCore/qmath.h>

#include <QFile>
#include <QTextStream>

// Distance between coordinates in kilometers
const double PI = 3.141592653589793238462;
static double DistanceBetweenCoordinates(double lat1d, double lon1d, double lat2d, double lon2d)
{
    // Sphere Earth

    double lat1r = lat1d * PI / 180;
    double lon1r = lon1d * PI / 180;
    double lat2r = lat2d * PI / 180;
    double lon2r = lon2d * PI / 180;

    double deltalon = lon1r - lon2r;

    double num = qSqrt(qPow(qCos(lat2r) * qSin(deltalon), 2) + qPow(qCos(lat1r) * qSin(lat2r) - qSin(lat1r) * qCos(lat2r) * qCos(deltalon), 2));
    double denum = qSin(lat1r) * qSin(lat2r) + qCos(lat1r) * qCos(lat2r) * qCos(deltalon);
    return 1000 * qAtan(num / denum) * 6372.795;
}

iodrv::iodrv(SystemStateViewModel *systemState)
    : distance_store_file("/media/milage.txt")
{
    //!!!!! TODO: ВРЕМЕННО
    this->systemState = systemState;

    gps_source = gps;
    read_socket_0 = -1;
    write_socket_0 = -1;
    write_socket_1 = -1;

    if( distance_store_file.open(QIODevice::ReadWrite) )
    {
        QTextStream distance_store_stream (&distance_store_file);
        stored_passed_distance = distance_store_stream.readLine().toInt();
        distance_store_file.close();
    }
    else
    {
        qDebug() << "Error open milage.txt!" << endl;
        stored_passed_distance = 0;
    }
    total_passed_distance = stored_passed_distance;

    pgd.lat = 0;

    c_speed = -1;
    c_speed_limit = -1;
    c_target_speed = -1;
    c_acceleration = -1;
    c_movement_direction = -1;
    c_trafficlight_light = -1;
    c_trafficlight_freq = -1;
    c_passed_distance = -1;
    c_epv_state = -1;
    c_epv_key = -1;
    c_driving_mode = -1;
    c_vigilance = -1;

    c_pressure_tc = -1;
    c_pressure_tm = -1;
    c_ssps_mode = 1;

    p_speed = -1;
    p_speed_limit = -1;
    p_target_speed = -1;
    p_acceleration = -1;
    p_movement_direction = -1;
    p_trafficlight_light = -1;
    p_trafficlight_freq = -1;
    p_passed_distance = -1;
    p_epv_state = -1;
    p_epv_key = -1;
    p_driving_mode = -1;
    p_vigilance = -1;

    p_pressure_tc = -1;
    p_pressure_tm = -1;
    p_ssps_mode = -1;

    c_lat = -1; c_lon = -1;
    c_ipd_hours = -1; c_ipd_mins = -1; c_ipd_secs = -1;
    c_ipd_year = -1; c_ipd_month = -1; c_ipd_day = -1;

    p_lat = -1; p_lon = -1;
    p_ipd_hours = -1; p_ipd_mins = -1; p_ipd_secs = -1;
    p_ipd_year = -1; p_ipd_month = -1; p_ipd_day = -1;

    timer_disp_state = NULL;
}

int iodrv::start(char* can_iface_name_0, char *can_iface_name_1, gps_data_source gps_datasource)
{
    gps_source = gps_datasource;

    // Инициализация сокетов
    if (init_sktcan(can_iface_name_0, can_iface_name_1) == 0)
    {
        //printf("Инициализация сокетов не удалась\n"); fflush(stdout);
        return 0;
    }
    // Петля чтения сокета
    QtConcurrent::run(this, &iodrv::read_canmsgs_loop);

    // Инициализация и начало асинхронного чтения с последовательного порта.
    // Если не выбрано другое.
    if (gps_source == gps)
    {
        if (init_serial_port() == 0)
        {
            return 0;
        }
    }

    // Инициализация и запуск таймеров
    init_timers();

    return 1;
}

int iodrv::init_sktcan(char* can_iface_name_0, char *can_iface_name_1)
{
    // По-хорошему все строки должны быть const char*.
    // http://stackoverflow.com/questions/7896645/c-deprecated-conversion-from-string-constant-to-char
    char* iface_name_0 = (char* )((can_iface_name_0 == NULL) ? "can0" : can_iface_name_0);
    char* iface_name_1 = (char* )((can_iface_name_1 == NULL) ? "can1" : can_iface_name_1);


    // Подготавливаем сокеты

    printf("Инициализация сокета чтения %s\n", iface_name_0); fflush(stdout);
    read_socket_0 = getSocket(iface_name_0);
    if(!read_socket_0)
    {
        return 0;
    }
    printf("Сокет чтения %s готов\n", iface_name_0); // TODO: Сообщение об общей готовности или ошибке должно быть в getSocket()
    fflush(stdout);

    printf("Инициализация сокета записи %s\n", iface_name_0); fflush(stdout);
    write_socket_0 = getSocket(iface_name_0);
    if(!write_socket_0)
    {
        return 0;
    }
    printf("Сокет записи %s готов\n", iface_name_0); fflush(stdout);

    printf("Инициализация сокета записи %s\n", iface_name_1); fflush(stdout);
    write_socket_1 = getSocket(iface_name_1);
    if(!write_socket_1)
    {
        return 0;
    }
    printf("Сокет записи %s готов\n", iface_name_1); fflush(stdout);

    return 1;
}

void iodrv::write_canmsg_async(int write_socket, can_frame* frame)
{
    // TODO: Передавать по значению и лочить вызов?
    // Создавать новый сокет на отправку и закрывать его.

    // Операция будет атомарной на одном сокете, пока не израсходован его внутренний буфер, который как минимум 512 байт.
    // Учитывая размер can_frame и плотность их отправки, его исчерпание маловероятно.

    //qDebug() << "cocure";
    //QtConcurrent::run(write_can_frame, write_socket, *frame);
    write_can_frame(write_socket, *frame);
}

void iodrv::read_canmsgs_loop()
{
    struct can_frame read_frame;
    while(true)
    {
        read_can_frame(read_socket_0, &read_frame);
        process_can_messages(&read_frame);
    }
}


int iodrv::process_can_messages(struct can_frame *frame)
{
    decode_speed(frame);
    decode_speed_limit(frame);
    decode_target_speed(frame);
    decode_acceleration(frame);

    decode_trafficlight_light(frame);
    decode_trafficlight_freq(frame);
    decode_passed_distance(frame);
    decode_epv_state(frame);
    decode_epv_key(frame);

    decode_driving_mode(frame);
    decode_vigilance(frame);
    decode_movement_direction(frame);
    decode_reg_tape_avl(frame);

    decode_pressure_tc_tm(frame);
    decode_ssps_mode(frame);

//    if(gps_source == can)
//    {
//        decode_mm_lat_lon(frame);
//        decode_ipd_datetime(frame);
//    }
}


// Скорость и ограничения

int iodrv::decode_speed(struct can_frame* frame)
{
    switch (can_decoder::decode_speed(frame, &c_speed))
    {
        case 1:
            if ((p_speed == -1) || (p_speed != -1 && p_speed != c_speed))
            {
                emit signal_speed_earth(c_speed);
//                printf("Speed: %f\n", c_speed); fflush(stdout);
            }
            p_speed = c_speed;
            break;
    }
}

int iodrv::decode_speed_limit(struct can_frame* frame)
{
    switch (can_decoder::decode_speed_limit(frame, &c_speed_limit))
    {
        case 1:
            if ((p_speed_limit == -1) || (p_speed_limit != -1 && p_speed_limit != c_speed_limit))
            {
                emit signal_speed_limit(c_speed_limit);
            }
            p_speed_limit = c_speed_limit;
            break;
    }
}

int iodrv::decode_target_speed(struct can_frame* frame)
{
    switch (can_decoder::decode_target_speed(frame, &c_target_speed))
    {
        case 1:
            if ((c_target_speed == -1) || (c_target_speed != -1 && p_target_speed != c_target_speed))
            {
                emit signal_target_speed(c_target_speed);
            }
            p_target_speed = c_target_speed;
            break;
    }
}

int iodrv::decode_acceleration(struct can_frame* frame)
{
    switch (can_decoder::decode_acceleration(frame, &c_acceleration))
    {
        case 1:
            if ((c_acceleration == -1) || (c_acceleration != -1 && p_acceleration != c_acceleration))
            {
                emit signal_acceleration(c_acceleration);
            }
            p_acceleration = c_acceleration;
            break;
    }
}




int iodrv::decode_movement_direction(struct can_frame* frame)
{
    switch (can_decoder::decode_movement_direction(frame, &c_movement_direction))
    {
        case 1:
            /*if ((p_movement_direction == -1) || (p_movement_direction != -1 && p_movement_direction != c_movement_direction))
            {
                emit signal_movement_direction(c_movement_direction);
            }*/
            emit signal_movement_direction(c_movement_direction);
            p_movement_direction = c_movement_direction;
            break;
    }
}

int iodrv::decode_trafficlight_light(struct can_frame* frame)
{
    switch (can_decoder::decode_trafficlight_light(frame, &c_trafficlight_light))
    {
        case 1:
            /*if ((p_trafficlight_light == -1) || (p_trafficlight_light != -1 && p_trafficlight_light != c_trafficlight_light))
            {
                emit signal_trafficlight_light(c_trafficlight_light);
            }*/
            emit signal_trafficlight_light(c_trafficlight_light);
            p_trafficlight_light = c_trafficlight_light;
            break;
    }
}

int iodrv::decode_trafficlight_freq(struct can_frame* frame)
{
    switch (can_decoder::decode_trafficlight_freq(frame, &c_trafficlight_freq))
    {
        case 1:
            if ((p_trafficlight_freq == -1) || (p_trafficlight_freq != -1 && p_trafficlight_freq != c_trafficlight_freq))
            {
                emit signal_trafficlight_freq(c_trafficlight_freq);
            }
            if (target_trafficlight_freq != c_trafficlight_freq)
            {
                this->slot_f_key_down();
                this->slot_f_key_up();
            }
            p_trafficlight_freq = c_trafficlight_freq;

            break;
    }
}

int iodrv::decode_passed_distance(struct can_frame* frame)
{
    switch (can_decoder::decode_passed_distance(frame, &c_passed_distance))
    {
        case 1:
            /*if ((p_passed_distance == -1) || (p_passed_distance != -1 && p_passed_distance != c_passed_distance))
            {
                emit signal_passed_distance(c_passed_distance);
            }
            p_passed_distance = c_passed_distance;*/

            // Общий одометр
            if (c_ssps_mode == 0 && p_passed_distance != -1 && c_passed_distance != -1)
            {
                total_passed_distance += (c_passed_distance - p_passed_distance);
                emit signal_passed_distance(total_passed_distance);
            }
            p_passed_distance = c_passed_distance;

            break;
    }
}

int iodrv::decode_epv_state(struct can_frame* frame)
{
    switch (can_decoder::decode_epv_state(frame, &c_epv_state))
    {
        case 1:
            if ((p_epv_state == -1) || (p_epv_state != -1 && p_epv_state != c_epv_state))
            {
                emit signal_epv_state(c_epv_state);
            }
            p_epv_state = c_epv_state;
            break;
    }
}

int iodrv::decode_epv_key(struct can_frame* frame)
{
    switch (can_decoder::decode_epv_key(frame, &c_epv_key))
    {
        case 1:
            if ((p_epv_key == -1) || (p_epv_key != -1 && p_epv_key != c_epv_key))
            {
                emit signal_epv_key(c_epv_key);
            }
            p_epv_key = c_epv_key;
            break;
    }
}

int iodrv::decode_mm_lat_lon(struct can_frame* frame)
{
    switch (can_decoder::decode_mm_lat_lon(frame, &c_lat, &c_lon))
    {
        case 1:
            if ((p_lat == -1) || (p_lat != -1 && p_lat != c_lat))
            {
                emit signal_lat(c_lat);
            }
            if ((p_lon == -1) || (p_lon != -1 && p_lon != c_lon))
            {
                emit signal_lon(c_lon);
            }
            p_lat = c_lat;
            p_lon = c_lon;
//            printf("Coord: lat = %f, lon = %f\n", c_lat, c_lon); fflush(stdout);
            break;
    }
}

int iodrv::decode_ipd_datetime(struct can_frame* frame)
{
    switch (can_decoder::decode_ipd_date(frame, &c_ipd_year, &c_ipd_month, &c_ipd_day, &c_ipd_hours, &c_ipd_mins, &c_ipd_secs))
    {
        case 1:
            if ((p_ipd_secs == -1) || (p_ipd_secs != -1 && p_ipd_secs != c_ipd_secs))
            {
                // printf("Time: %d:%d:%d\n", c_ipd_hours, c_ipd_mins, c_ipd_secs); fflush(stdout);

                QString time = QString("%1:%2:%3").arg(c_ipd_hours, 2, 10, QChar('0')).arg(c_ipd_mins, 2, 10, QChar('0')).arg(c_ipd_secs, 2, 10, QChar('0'));
                emit signal_time(time);
            }
            if ((p_ipd_day == -1) || (p_ipd_day != -1 && p_ipd_day != c_ipd_day))
            {
                QString monthString[12] = {"января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};
                QString date = QString("%1 %2 %3").arg(c_ipd_day, 2, 10, QChar('0')).arg(monthString[c_ipd_month-1]).arg(c_ipd_year, 2, 10, QChar('0'));

                emit signal_date(date);
            }
            p_ipd_secs = c_ipd_secs;
            p_ipd_day = c_ipd_day;
            break;
    }
}

int iodrv::decode_driving_mode(struct can_frame* frame)
{
    switch (can_decoder::decode_driving_mode(frame, &c_driving_mode))
    {
        case 1:
            if ((p_driving_mode == -1) || (p_driving_mode != -1 && p_driving_mode != c_driving_mode))
            {
                emit signal_driving_mode(c_driving_mode);
            }
            if (target_driving_mode != c_driving_mode)
            {
                this->slot_rmp_key_down();
            }
            p_driving_mode = c_driving_mode;

            break;
    }
}

int iodrv::decode_vigilance(struct can_frame* frame)
{
    switch (can_decoder::decode_vigilance(frame, &c_vigilance))
    {
        case 1:
            if ((p_vigilance == -1) || (p_vigilance != -1 && p_vigilance != c_vigilance))
            {
                emit signal_vigilance(c_vigilance);
            }
            p_vigilance = c_vigilance;
            break;
    }
}

int iodrv::decode_reg_tape_avl(struct can_frame* frame)
{
    switch (can_decoder::decode_reg_tape_avl(frame, &c_reg_tape_avl))
    {
        case 1:
            if ((p_reg_tape_avl == -1) || (p_reg_tape_avl != -1 && p_reg_tape_avl != c_reg_tape_avl))
            {
                emit signal_reg_tape_avl(c_reg_tape_avl);
            }
            p_reg_tape_avl = c_reg_tape_avl;
            break;
    }
}

int iodrv::decode_pressure_tc_tm(struct can_frame* frame)
{
    switch (can_decoder::decode_pressure_tc_tm(frame, &c_pressure_tc, &c_pressure_tm))
    {
        case 1:
            if ((p_pressure_tc == -1) || (p_pressure_tc != -1 && p_pressure_tc != c_pressure_tc))
            {
                emit signal_pressure_tc(QString::number(c_pressure_tc, 'f', 2));
            }
            p_pressure_tc = c_pressure_tc;

            if ((p_pressure_tm == -1) || (p_pressure_tm != -1 && p_pressure_tm != c_pressure_tm))
            {
                emit signal_pressure_tm(QString::number(c_pressure_tm, 'f', 2));
            }
            p_pressure_tm = c_pressure_tm;

            break;
    }
}

int iodrv::decode_ssps_mode(struct can_frame* frame)
{
    switch (can_decoder::decode_ssps_mode(frame, &c_ssps_mode))
    {
        case 1:
            if ((p_ssps_mode == -1) || (p_ssps_mode != -1 && p_ssps_mode != c_ssps_mode))
            {
                emit signal_ssps_mode(c_ssps_mode);
            }
            p_ssps_mode = c_ssps_mode;
            break;
    }
}




int iodrv::init_serial_port()
{
#ifdef WITH_SERIALPORT
    // В каком место разместить соединение сигнала и слота?
    connect(&serial_port, SIGNAL(readyRead()), this, SLOT(slot_serial_ready_read()));

    QList<SerialPortInfo> spinfo = SerialPortInfo::availablePorts();
    if (spinfo.count() == 0)
    {
        fprintf(stderr, "Не найдено ни одного последовательного порта\n"); fflush(stderr);
        // Для обхода бага библиотеки QtSerialPort под arm обходим эту проверку
        //return 0;
    }

    //serial_port.setPort(spinfo.at(0));
    serial_port.setPort("/dev/ttySAC1");
    serial_port.setDataBits(SerialPort::Data8);
    serial_port.setRate(SerialPort::Rate115200);
    serial_port.setParity(SerialPort::NoParity);
    serial_port.setStopBits(SerialPort::OneStop);
    serial_port.setFlowControl(SerialPort::NoFlowControl);

    serial_port.open(QIODevice::ReadOnly);

    return 1;
#else
    printf("Отключена компиляция SerialPort; используйте WITH_SERIALPORT.\n"); fflush(stdout);

    return 1;
#endif
}

void iodrv::slot_serial_ready_read()
{
#ifdef WITH_SERIALPORT
    if (serial_port.canReadLine())
    {
        gps_data gd;    // TODO: Сделать глобальной?

        QString nmeaMessage = QString(serial_port.readLine());

        if ( nmea::decode_nmea_message(nmeaMessage, &gd) )
        {
            QString time = QString("%1:%2:%3").arg(gd.hours, 2, 10, QChar('0')).arg(gd.minutes, 2, 10, QChar('0')).arg(gd.seconds, 2, 10, QChar('0'));
            emit signal_time(time);

            QString monthString[13] = {"n/a", "января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};
            gd.month = ( gd.month >= 1 && gd.month <= 12 ) ? gd.month : 0;
            QString date = QString("%1 %2 %3").arg(gd.day, 2, 10, QChar('0')).arg(monthString[gd.month]).arg(gd.year, 2, 10, QChar('0'));
            emit signal_date(date);

            emit signal_lat(gd.lat);
            emit signal_lon(gd.lon);


            static double speed_old = 0;

            if (gd.speed < 1.2)
                gd.speed = 0;

            if (speed_old != -1000 && abs(speed_old - gd.speed) > 5)
            {
                gd.speed = speed_old;
                speed_old = -1000;
            }
            else
                speed_old = gd.speed;

            emit signal_speed_sky(gd.is_reliable ? gd.speed : -1);

            if (pgd.lat != 0)
            {
                total_passed_distance += DistanceBetweenCoordinates(gd.lat, gd.lon, pgd.lat, pgd.lon);

                if ( (total_passed_distance - stored_passed_distance) >= 100 )
                {
                    if( distance_store_file.open(QIODevice::ReadWrite) )
                    {
                        QTextStream distance_store_stream (&distance_store_file);
                        distance_store_stream << int(total_passed_distance) << endl;
                        distance_store_stream.flush();
                        distance_store_file.close();
                        stored_passed_distance = total_passed_distance;
                    }
                    else
                    {
                        qDebug() << "Error open milage.txt!" << endl;
                    }
                }

                emit signal_passed_distance(total_passed_distance);
            }

            pgd = gd;

            // Отправка в CAN

            wframe_mmaltlon = can_encoder::encode_mm_alt_long(gd.lat, gd.lon, (bool)gd.is_reliable);
            wframe_ipddate = can_encoder::encode_ipd_date(gd.year, gd.month, gd.day, gd.hours, gd.minutes, gd.seconds);
            wframe_mmdata = can_encoder::encode_mm_data(qRound(gd.speed));

            write_canmsg_async(write_socket_0, &wframe_mmaltlon);
            write_canmsg_async(write_socket_1, &wframe_mmaltlon);
            write_canmsg_async(write_socket_0, &wframe_ipddate);
            write_canmsg_async(write_socket_1, &wframe_ipddate);
            write_canmsg_async(write_socket_0, &wframe_mmdata);
            write_canmsg_async(write_socket_1, &wframe_mmdata);
        }
    }
#endif
}


void iodrv::init_timers()
{
    timer_disp_state = new QTimer(this);
    connect(timer_disp_state, SIGNAL(timeout()), this, SLOT(slot_can_write_disp_state()));
    timer_disp_state->start(500);
}

void iodrv::slot_can_write_disp_state()
{
    can_frame frame_a = can_encoder::encode_disp_state_a();
    write_canmsg_async(write_socket_0, &frame_a);

    can_frame frame_b = can_encoder::encode_disp_state_b();
    write_canmsg_async(write_socket_1, &frame_b);
}

void iodrv::slot_f_key_down()
{
    can_frame frame = can_encoder::encode_sys_key(is_pressed, 0x1C);
    write_canmsg_async(write_socket_0, &frame);
    write_canmsg_async(write_socket_1, &frame);

    target_trafficlight_freq = systemState->getAlsnFreqTarget();
}

void iodrv::slot_f_key_up()
{
    can_frame frame = can_encoder::encode_sys_key(is_released, 0x1C);
    write_canmsg_async(write_socket_0, &frame);
    write_canmsg_async(write_socket_1, &frame);
}

void iodrv::slot_vk_key_down()
{
    can_frame frame = can_encoder::encode_sys_key(is_pressed, 0x14);
    write_canmsg_async(write_socket_0, &frame);
    write_canmsg_async(write_socket_1, &frame);
}

void iodrv::slot_vk_key_up()
{
    can_frame frame = can_encoder::encode_sys_key(is_released, 0x14);
    write_canmsg_async(write_socket_0, &frame);
    write_canmsg_async(write_socket_1, &frame);
}

void iodrv::slot_rmp_key_down()
{
    can_frame frame = can_encoder::encode_sys_key(is_pressed, 0x16);
    write_canmsg_async(write_socket_0, &frame);
    write_canmsg_async(write_socket_1, &frame);

    this->target_driving_mode = systemState->getDriveModeTarget();
}

void iodrv::slot_rmp_key_up()
{
    can_frame frame = can_encoder::encode_sys_key(is_released, 0x16);
    write_canmsg_async(write_socket_0, &frame);
    write_canmsg_async(write_socket_1, &frame);
}


SpeedAgregator::SpeedAgregator()
        : currentSpeed(0), currentSpeedIsValid(false)
    {}


void SpeedAgregator::getSpeedFromSky (double speed)
{
    if ( speed > 0 ) // достоверность
    {
        setSpeedIsValid( !(
                        speed > minSpeedSkyAccount &&
                        abs(speed - currentSpeed) > maxAllowDeltaSpeed
                            )
                );
    }
    qDebug() << "Speed | " << speed << " | " << currentSpeed;
}

void SpeedAgregator::getSpeedFromEarth (double speed)
{
    if (speed != currentSpeed)
    {
        currentSpeed = speed;
        emit speedChanged(speed);
    }
}

void SpeedAgregator::setSpeedIsValid (bool isValid)
{
    if (currentSpeedIsValid != isValid)
    {
        currentSpeedIsValid = isValid;
        emit speedIsValidChanged(isValid);
    }
}


#endif
