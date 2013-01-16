#ifndef IODRV_H
#define IODRV_H


#include "iodrvmain.h"
#include "sktcan.h"
#include "endecs.h"


QT_USE_NAMESPACE_SERIALPORT

enum gps_data_source
{
    gps,
    can
};

class iodrv : public QObject
{
    Q_OBJECT

public:
    iodrv();

public:
    int start(char* can_iface_name, gps_data_source gps_datasource = gps);

signals:
    // Сигналы вызываются немедленно или у них есть внутренняя очередь, так что они могут передать изменённое значение?
    void signal_speed(double speed);
    void signal_speed_limit(int speed_limit);
    void signal_stop_flag(int stop_flag);
    void signal_movement_direction(int movement_direction);
    void signal_trafficlight_light(int trafficlight_light);
    void signal_trafficlight_freq(int trafficlight_freq);
    void signal_passed_distance(int passed_distance);
    void signal_epv_state(int epv_state);
    void signal_epv_key(int epv_key);

    void signal_lat(double lat);
    void signal_lon(double lon);
    void signal_time(QString time);
    void signal_date(QString date);

public slots:
    void slot_fkey_down();
    void slot_fkey_up();

private slots:
    void slot_serial_ready_read();
    void slot_can_write_disp_state();

private:
    gps_data_source gps_source = gps;
    int read_socket = -1;
    int write_socket = -1;
    //struct can_frame read_frame;
    //struct can_frame write_frame;
    int init_sktcan(char* can_iface_name);
    void read_canmsgs_loop();
    // TODO: Контролировать были ли сообщения отосланы и если нет, то что-нибудь делать.
    void write_canmsg_async(can_frame* frame);


    // По-хорошему, эти переменные и работающие с ними функции должны быть объявлены в нити, обрабатывающей read_can_message.
    double c_speed = -1;
    int c_speed_limit = -1;
    int c_stop_flag = -1;
    int c_movement_direction = -1;
    int c_trafficlight_light = -1;
    int c_trafficlight_freq = -1;
    int c_passed_distance = -1;
    int c_epv_state = -1;
    int c_epv_key = -1;

    double p_speed = -1;
    int p_speed_limit = -1;
    int p_stop_flag = -1;
    int p_movement_direction = -1;
    int p_trafficlight_light = -1;
    int p_trafficlight_freq = -1;
    int p_passed_distance = -1;
    int p_epv_state = -1;
    int p_epv_key = -1;


    int decode_speed(struct can_frame* frame);
    int decode_speed_limit(struct can_frame* frame);
    int decode_stop_flag(struct can_frame* frame);
    int decode_movement_direction(struct can_frame* frame);
    int decode_trafficlight_light(struct can_frame* frame);
    int decode_trafficlight_freq(struct can_frame* frame);
    int decode_passed_distance(struct can_frame* frame);
    int decode_epv_state(struct can_frame* frame);
    int decode_epv_key(struct can_frame* frame);
    int decode_mm_lat_lon(struct can_frame* frame);
    int decode_ipd_datetime(struct can_frame* frame);

    int process_can_messages(struct can_frame* frame);


    SerialPort serial_port;
    int init_serial_port();

    double c_lat = -1; double c_lon = -1;
    int c_ipd_hours = -1; int c_ipd_mins = -1; int c_ipd_secs = -1;
    int c_ipd_year = -1; int c_ipd_month = -1; int c_ipd_day = -1;

    double p_lat = -1; double p_lon = -1;
    int p_ipd_hours = -1; int p_ipd_mins = -1; int p_ipd_secs = -1;
    int p_ipd_year = -1; int p_ipd_month = -1; int p_ipd_day = -1;


    QTimer* timer_disp_state = NULL;
    void init_timers();
};

#endif // IODRV_H
