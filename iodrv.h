#ifndef IODRV_H
#define IODRV_H

#include <QFile>
#include <QTextStream>

#include "qtCanLib/ICan.h"

#include "iodrvmain.h"
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
    iodrv(ICan *onCan, QObject *parent = 0);


public:
    int start(gps_data_source gps_datasource = gps_data_source_gps);

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
    void signal_modules_activity(QString modulesActivity);
    //Одометр
    void signal_passed_distance(int passed_distance);
    void signal_orig_passed_distance(int orig_passed_distance);
    //Светофоры
    void signal_trafficlight_freq(int trafficlight_freq);
    void signal_trafficlight_freq_target (int trafficlight_freq_target);
    //Движение
    void signal_vigilance(bool vigilance);
    void signal_movement_direction(int movement_direction);
    void signal_reg_tape_avl(bool reg_tape_avl);

public slots:
    void slot_send_message(CanFrame frame);

//    void slot_f_key_down();
//    void slot_f_key_up();
//    void slot_vk_key_down();
//    void slot_vk_key_up();
//    void slot_rmp_key_down();
//    void slot_rmp_key_up();

private slots:
    void process_can_messages(CanFrame frame);
    void slot_serial_ready_read();

private:
    ICan *can;
    gps_data_source gps_source;
    int read_socket_0;
    int write_socket_0;
    int write_socket_1;
    //struct can_frame read_frame;
    //struct can_frame write_frame;
    int init_sktcan();

    // TODO: Контролировать были ли сообщения отосланы и если нет, то что-нибудь делать.
    void write_canmsg_async(int socket, const CanFrame &frame);

    QFile distance_store_file;
    double total_passed_distance;
    double stored_passed_distance;
    gps_data pgd;

    CanFrame wframe_mmaltlon;
    CanFrame wframe_ipddate;
    CanFrame wframe_mmdata;
    CanFrame wframe_ipdstate;

    // По-хорошему, эти переменные и работающие с ними функции должны быть объявлены в нити, обрабатывающей read_can_message.
    double c_speed;
    int c_speed_limit;
    int c_target_speed;
    double c_acceleration;

    int c_movement_direction;
    int c_passed_distance;
    int c_orig_passed_distance;
    int c_epv_state;
    int c_epv_key;
    ModulesActivity c_modulesActivity;

    int c_driving_mode;
    int c_vigilance;
    int c_reg_tape_avl;

    double c_pressure_tc;
    double c_pressure_tm;
    int c_is_on_road;

    double p_speed;
    int p_speed_limit;
    int p_target_speed;
    double p_acceleration;

    int p_movement_direction;
    int p_passed_distance;
    int p_epv_state;
    int p_epv_key;
    ModulesActivity p_modulesActivity;

    int p_driving_mode;
    int p_vigilance;
    int p_reg_tape_avl;

    double p_pressure_tc;
    double p_pressure_tm;
    int p_is_on_road;

    int c_ssps_mode; int p_ssps_mode;
    int c_in_traction; int p_in_traction;

    int decode_speed(const CanFrame &frame);
    int decode_speed_limit(const CanFrame &frame);
    int decode_target_speed(const CanFrame &frame);
    int decode_acceleration(const CanFrame &frame);
    int decode_passed_distance(const CanFrame &frame);

    int decode_movement_direction(const CanFrame &frame);
    int decode_trafficlight_freq(const CanFrame &frame);
    int decode_orig_passed_distance (const CanFrame &frame);
    int decode_modules_activity(const CanFrame &frame);
//    int decode_ipd_datetime(const CanFrame &frame);
    int decode_vigilance(const CanFrame &frame);
    int decode_reg_tape_avl(const CanFrame &frame);

    int c_ipd_hours; int c_ipd_mins; int c_ipd_secs;
    int c_ipd_year; int c_ipd_month; int c_ipd_day;

    int p_ipd_hours; int p_ipd_mins; int p_ipd_secs;
    int p_ipd_year; int p_ipd_month; int p_ipd_day;

    void init_timers();

    // Переменные отслеживания состояния, когда задаётся целевое значение, но до его «применения» могут приходить фактические значения.
    int target_trafficlight_freq;
    int target_driving_mode;
};

/*class rmp_revolver : public QObject
{
    //Q_OBJECT

public:
    //void request_driving_mode(int requesting_driving_mode, int current_ssps_mode);


//private:
};*/

#endif // IODRV_H
