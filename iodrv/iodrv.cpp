#include "iodrv.h"

iodrv::iodrv()
{
    gps_source = gps;
    read_socket = -1;
    write_socket = -1;

    c_speed = -1;
    c_speed_limit = -1;
    c_stop_flag = -1;
    c_movement_direction = -1;
    c_trafficlight_light = -1;
    c_trafficlight_freq = -1;
    c_passed_distance = -1;
    c_epv_state = -1;
    c_epv_key = -1;

    p_speed = -1;
    p_speed_limit = -1;
    p_stop_flag = -1;
    p_movement_direction = -1;
    p_trafficlight_light = -1;
    p_trafficlight_freq = -1;
    p_passed_distance = -1;
    p_epv_state = -1;
    p_epv_key = -1;

    c_lat = -1; c_lon = -1;
    c_ipd_hours = -1; c_ipd_mins = -1; c_ipd_secs = -1;
    c_ipd_year = -1; c_ipd_month = -1; c_ipd_day = -1;

    p_lat = -1; p_lon = -1;
    p_ipd_hours = -1; p_ipd_mins = -1; p_ipd_secs = -1;
    p_ipd_year = -1; p_ipd_month = -1; p_ipd_day = -1;

    timer_disp_state = NULL;
}

int iodrv::start(char* can_iface_name, gps_data_source gps_datasource)
{
    gps_source = gps_datasource;

    // Инициализация сокетов
    if (init_sktcan(can_iface_name) == 0)
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

int iodrv::init_sktcan(char* can_iface_name)
{
    // По-хорошему все строки должны быть const char*.
    // http://stackoverflow.com/questions/7896645/c-deprecated-conversion-from-string-constant-to-char
    char* iface_name = (char* )((can_iface_name == NULL) ? "can0" : can_iface_name);


    // Подготавливаем сокеты

    printf("Инициализация сокета чтения\n"); fflush(stdout);
    read_socket = getSocket(iface_name);
    if(!read_socket)
    {
        return 0;
    }
    printf("Сокет чтения готов\n"); // TODO: Сообщение об общей готовности или ошибке должно быть в getSocket()
    fflush(stdout);

    printf("Инициализация сокета записи\n"); fflush(stdout);
    write_socket = getSocket(iface_name);
    if(!write_socket)
    {
        return 0;
    }
    printf("Сокет записи готов\n"); fflush(stdout);

    return 1;
}

void iodrv::write_canmsg_async(can_frame* frame)
{
    //TODO: Передавать по значению и лочить вызов?
    // Создавать новый сокет на отправку и закрывать его.

    // Операция будет атомарной на одном сокете, пока не израсходован его внутренний буфер, который как минимум 512 байт.
    // Учитывая размер can_frame и плотность их отправки, его исчерпание маловероятно.

    QtConcurrent::run(write_can_frame, write_socket, frame);
}

void iodrv::read_canmsgs_loop()
{
    struct can_frame read_frame;
    while(true)
    {
        read_can_frame(read_socket, &read_frame);
        process_can_messages(&read_frame);
    }
}


int iodrv::process_can_messages(struct can_frame *frame)
{
    decode_speed(frame);
    decode_speed_limit(frame);
    decode_stop_flag(frame);
    decode_movement_direction(frame);
    decode_trafficlight_light(frame);
    decode_trafficlight_freq(frame);
    decode_passed_distance(frame);
    decode_epv_state(frame);
    decode_epv_key(frame);

    if(gps_source == can)
    {
        decode_mm_lat_lon(frame);
        decode_ipd_datetime(frame);
    }
}

int iodrv::decode_speed(struct can_frame* frame)
{
    switch (can_decoder::decode_speed(frame, &c_speed))
    {
        case 1:
            if ((p_speed == -1) || (p_speed != -1 && p_speed != c_speed))
            {
                emit signal_speed(c_speed);
                //printf("Speed: %f\n", c_speed); fflush(stdout);
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

int iodrv::decode_stop_flag(struct can_frame* frame)
{
    switch (can_decoder::decode_stop_flag(frame, &c_stop_flag))
    {
        case 1:
            if ((p_stop_flag == -1) || (p_stop_flag != -1 && p_stop_flag != c_stop_flag))
            {
                emit signal_stop_flag(c_stop_flag);
            }
            p_stop_flag = c_stop_flag;
            break;
    }
}

int iodrv::decode_movement_direction(struct can_frame* frame)
{
    switch (can_decoder::decode_movement_direction(frame, &c_movement_direction))
    {
        case 1:
            if ((p_movement_direction == -1) || (p_movement_direction != -1 && p_movement_direction != c_movement_direction))
            {
                emit signal_movement_direction(c_movement_direction);
            }
            p_movement_direction = c_movement_direction;
            break;
    }
}

int iodrv::decode_trafficlight_light(struct can_frame* frame)
{
    switch (can_decoder::decode_trafficlight_light(frame, &c_trafficlight_light))
    {
        case 1:
            if ((p_trafficlight_light == -1) || (p_trafficlight_light != -1 && p_trafficlight_light != c_trafficlight_light))
            {
                emit signal_trafficlight_light(c_trafficlight_light);
            }
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
            p_trafficlight_freq = c_trafficlight_freq;
            break;
    }
}

int iodrv::decode_passed_distance(struct can_frame* frame)
{
    switch (can_decoder::decode_passed_distance(frame, &c_passed_distance))
    {
        case 1:
            if ((p_passed_distance == -1) || (p_passed_distance != -1 && p_passed_distance != c_passed_distance))
            {
                emit signal_passed_distance(c_passed_distance);
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
            //printf("Coord: lat = %f, lon = %f\n", c_lat, c_lon); fflush(stdout);
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

                QString time = QString("%1:%2:%3").arg(c_ipd_hours, 2, '0').arg(c_ipd_mins, 2, '0').arg(c_ipd_secs, 2, '0');
                emit signal_time(time);
            }
            if ((p_ipd_day == -1) || (p_ipd_day != -1 && p_ipd_day != c_ipd_day))
            {
                QString date = QString("%1/%2/%3").arg(c_ipd_day, 2, '0').arg(c_ipd_month, 2, '0').arg(c_ipd_year, 2, '0');
                emit signal_date(date);
            }
            p_ipd_secs = c_ipd_secs;
            p_ipd_day = c_ipd_day;
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
        return 0;
    }

    //serial_port.setPort(spinfo.at(0));
    serial_port.setPort("/dev/ttyUSB0");
    serial_port.setDataBits(SerialPort::Data8);
    serial_port.setRate(SerialPort::Rate115200);
    serial_port.setParity(SerialPort::NoParity);
    serial_port.setStopBits(SerialPort::OneStop);
    serial_port.setFlowControl(SerialPort::NoFlowControl);

    serial_port.open(QIODevice::ReadOnly);

    return 1;
#else
    printf("Отключена компиляция SerialPort; используйте WITH_SERIALPORT.\n"); fflush(stdout);
    return 0;
#endif
}

void iodrv::slot_serial_ready_read()
{
#ifdef WITH_SERIALPORT
    if (serial_port.canReadLine())
    {
        gps_data gd;    // TODO: Сделать глобальной?
        can_frame wframe_mmaltlon;
        can_frame wframe_ipddate;

        nmea::decode_nmea_message(QString(serial_port.readLine()), &gd);

        wframe_mmaltlon = can_encoder::encode_mm_alt_long(gd.lat, gd.lon, (bool)gd.is_reliable);
        wframe_ipddate = can_encoder::encode_ipd_date(gd.year, gd.month, gd.day, gd.hours, gd.minutes, gd.seconds);

        write_canmsg_async(&wframe_mmaltlon);
        write_canmsg_async(&wframe_ipddate);

        if (gd.is_reliable)
        {
            QString time = QString("%1:%2:%3").arg(gd.hours, 2, '0').arg(gd.minutes, 2, '0').arg(gd.seconds, 2, '0');
            emit signal_time(time);

            QString date = QString("%1/%2/%3").arg(gd.day, 2, '0').arg(gd.month, 2, '0').arg(gd.year, 2, '0');
            emit signal_date(date);

            emit signal_lat(gd.lat);
            emit signal_lon(gd.lon);
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
    can_frame frame = can_encoder::encode_disp_state_a();
    write_canmsg_async(&frame);
}

void iodrv::slot_fkey_down()
{
    can_frame frame = can_encoder::encode_sys_key(is_pressed);
    write_canmsg_async(&frame);
}

void iodrv::slot_fkey_up()
{
    can_frame frame = can_encoder::encode_sys_key(is_released);
    write_canmsg_async(&frame);
}
