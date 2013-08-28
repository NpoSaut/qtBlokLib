#if defined WITH_CAN || defined WITH_SERIAL

#ifndef IODRV_H
#define IODRV_H

#include <QFile>
#include <QTextStream>

//!!!!! TODO: ВРЕМЕННО
#include "systemstateviewmodel.h"

#include "iodrvmain.h"
//#include "sktcan.h"
#include "qtCanLib/canframe.h"
#include "endecs.h"
#include "modulesactivity.h"


#ifdef WITH_SERIALPORT
QT_USE_NAMESPACE
#endif

enum gps_data_source
{
    gps_data_source_gps,
    gps_data_source_can
};

class iodrv : public QObject
{
    Q_OBJECT

public:
    //!!!!! TODO: ВРЕМЕННО
    iodrv(SystemStateViewModel *systemState);


public:
    int start(char* can_iface_name_0, char* can_iface_name_1, gps_data_source gps_datasource = gps_data_source_gps);

signals:
    // Сигналы вызываются немедленно или у них есть внутренняя очередь, так что они могут передать изменённое значение

    // Все сообщения
    void signal_new_message(CanFrame frame);

    //Скорость и ограничения
    void signal_speed_earth(double speed);
    void signal_speed_sky(double speed);
    void signal_speed_limit(int speed_limit);
    void signal_target_speed(int target_speed);
    void signal_acceleration(double acceleration);
    //Состояние системы
    void signal_epv_released(bool epv_state);
    void signal_epv_key(bool epv_key);
    void signal_modules_activity(QString modulesActivity);
    //Одометр
    void signal_passed_distance(int passed_distance);
    void signal_orig_passed_distance(int orig_passed_distance);
    //Светофоры
    void signal_trafficlight_light(int trafficlight_light);
    void signal_trafficlight_freq(int trafficlight_freq);
    //Движение
    void signal_driving_mode(int driving_mode);
    void signal_vigilance(bool vigilance);
    void signal_movement_direction(int movement_direction);
    void signal_reg_tape_avl(bool reg_tape_avl);

    void signal_autolock_type(int autolock_type);
    void signal_autolock_type_target(int autolock_type);

    void signal_pressure_tc(QString pressure_tc);
    void signal_pressure_tm(QString pressure_tm);
    void signal_is_on_road(bool is_on_road);
    void signal_ssps_mode(int ssps_mode);
    void signal_traction(bool in_traction);
    void signal_iron_wheels(bool iron_wheels);

    void signal_lat(double lat);
    void signal_lon(double lon);
    void signal_lat_lon(double lat, double lon);
    void signal_time(QString time);
    void signal_date(QString date);

public slots:
    void slot_send_message(CanFrame frame);

    void slot_f_key_down();
    void slot_f_key_up();
    void slot_vk_key_down();
    void slot_vk_key_up();
    void slot_rmp_key_down();
    void slot_rmp_key_up();
    void slot_autolock_type_target_changed(int value);

private slots:
    void process_can_messages(CanFrame frame);
    void slot_serial_ready_read();
    void slot_can_write_disp_state();

private:
    gps_data_source gps_source;
    int read_socket_0;
    int write_socket_0;
    int write_socket_1;
    //struct can_frame read_frame;
    //struct can_frame write_frame;
    int init_sktcan(char* can_iface_name_0, char* can_iface_name_1);

    // TODO: Контролировать были ли сообщения отосланы и если нет, то что-нибудь делать.
    void write_canmsg_async(int socket, can_frame* frame);

    QFile distance_store_file;
    double total_passed_distance;
    double stored_passed_distance;
    gps_data pgd;

    can_frame wframe_mmaltlon;
    can_frame wframe_ipddate;
    can_frame wframe_mmdata;
    can_frame wframe_ipdstate;

    // По-хорошему, эти переменные и работающие с ними функции должны быть объявлены в нити, обрабатывающей read_can_message.
    double c_speed;
    int c_speed_limit;
    int c_target_speed;
    double c_acceleration;

    int c_movement_direction;
    int c_trafficlight_light;
    int c_trafficlight_freq;
    int c_passed_distance;
    int c_orig_passed_distance;
    int c_epv_state;
    int c_epv_key;
    ModulesActivity c_modulesActivity;

    int c_driving_mode;
    int c_vigilance;
    int c_reg_tape_avl;

    int c_autolock_type;
    int c_autolock_type_target;

    double c_pressure_tc;
    double c_pressure_tm;
    int c_is_on_road;

    double p_speed;
    int p_speed_limit;
    int p_target_speed;
    double p_acceleration;

    int p_movement_direction;
    int p_trafficlight_light;
    int p_trafficlight_freq;
    int p_passed_distance;
    int p_epv_state;
    int p_epv_key;
    ModulesActivity p_modulesActivity;

    int p_driving_mode;
    int p_vigilance;
    int p_reg_tape_avl;

    int p_autolock_type;
    int p_autolock_type_target;

    double p_pressure_tc;
    double p_pressure_tm;
    int p_is_on_road;

    int c_ssps_mode; int p_ssps_mode;
    int c_in_traction; int p_in_traction;

    int decode_speed(struct can_frame* frame);
    int decode_speed_limit(struct can_frame* frame);
    int decode_target_speed(struct can_frame* frame);
    int decode_acceleration(struct can_frame* frame);

    int decode_movement_direction(struct can_frame* frame);
    int decode_trafficlight_light(struct can_frame* frame);
    int decode_trafficlight_freq(struct can_frame* frame);
    int decode_passed_distance(struct can_frame* frame);
    int decode_orig_passed_distance (struct can_frame* frame);
    int decode_epv_state(struct can_frame* frame);
    int decode_epv_key(struct can_frame* frame);
    int decode_modules_activity(struct can_frame* frame);
    int decode_mm_lat_lon(struct can_frame* frame);
    int decode_ipd_datetime(struct can_frame* frame);
    int decode_driving_mode(struct can_frame* frame);
    int decode_vigilance(struct can_frame* frame);
    int decode_reg_tape_avl(struct can_frame* frame);

    int decode_pressure_tc_tm(struct can_frame* frame);

    int decode_ssps_mode(struct can_frame* frame);
    int decode_traction(struct can_frame* frame);

    int decode_is_on_road(struct can_frame* frame);

    int decode_autolock_type(struct can_frame* frame);
    int set_autolock_type(int autolock_type);


#ifdef WITH_SERIALPORT
    QSerialPort serial_port;
#endif
    int init_serial_port();


    double c_lat; double c_lon;
    int c_ipd_hours; int c_ipd_mins; int c_ipd_secs;
    int c_ipd_year; int c_ipd_month; int c_ipd_day;

    double p_lat; double p_lon;
    int p_ipd_hours; int p_ipd_mins; int p_ipd_secs;
    int p_ipd_year; int p_ipd_month; int p_ipd_day;


    QTimer* timer_disp_state;
    void init_timers();

    // Переменные отслеживания состояния, когда задаётся целевое значение, но до его «применения» могут приходить фактические значения.
    int target_trafficlight_freq;
    int target_driving_mode;

    //!!!!! TODO: ВРЕМЕННО
    SystemStateViewModel *systemState;
};

class SpeedAgregator: public QObject
{
    Q_OBJECT
public:
    SpeedAgregator();

    static constexpr double minSpeedSkyAccount = 8;
    static constexpr double maxAllowDeltaSpeed = 4;

public slots:
    void getSpeedFromSky (double speed);
    void getSpeedFromEarth (double speed);
    void getIsOnRoad (bool isOnRoad);

signals:
    void speedChanged (double speed);
    void speedIsValidChanged (bool isValid);

private:
    double currentSpeedFromSky;
    double currentSpeedFromEarth;
    bool currentSpeedIsValid;
    bool onRails;

    void getNewSpeed (double speedFromSky, double speedFromEarth);
    void setSpeedIsValid (bool isValid);
};




/*class rmp_revolver : public QObject
{
    //Q_OBJECT

public:
    //void request_driving_mode(int requesting_driving_mode, int current_ssps_mode);


//private:
};*/

class rmp_key_handler : public QObject
{
    Q_OBJECT

public:
    rmp_key_handler();

private:
    int get_next_driving_mode(int driving_mode, int ssps_mode);

    int previous_ssps_mode;
    int actual_ssps_mode;
    int previous_driving_mode;
    int actual_driving_mode;
    int target_driving_mode;
    int req_count;
    void request_next_driving_mode();
    void request_driving_mode(int driving_mode);

    bool start;

public slots:
    // Interface
    void rmp_key_pressed();
    // IO
    void ssps_mode_received(int actual_ssps_mode);
    void driving_mode_received(int driving_mode);

signals:
    // Interface
    void target_driving_mode_changed(int driving_mode);
    void actual_driving_mode_changed(int driving_mode);
    // Interface, IO
    // void driving_mode_request_failed();
    // IO
    void rmp_key_pressed_send();
};

#endif // IODRV_H

#endif
