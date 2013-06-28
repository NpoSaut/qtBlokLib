#if defined WITH_CAN || defined WITH_SERIAL

#ifndef ENDECS_H
#define ENDECS_H

#include <QString>

#include "iodrvmain.h"
#include "modulesactivity.h"

enum key_state
{
    is_pressed,
    is_released
};

class can_encoder
{
public:
    static can_frame encode_mm_alt_long(double lat, double lon, bool reliability);
    static can_frame encode_ipd_date(int year, int month, int day, int hours, int minutes, int seconds);
    static can_frame encode_disp_state_a();
    static can_frame encode_disp_state_b();
    static can_frame encode_sys_key(key_state k_state, int key_code);
    static can_frame encode_mm_data(int speed, int milage);
    static can_frame encode_ipd_state( double speed, int distance, bool reliable );
    static can_frame encode_autolock_set_message (int autolock_type);
};

class can_decoder
{
public:
    static int decode_speed(struct can_frame* frame, double* speed);
    static int decode_speed_limit(struct can_frame* frame, int* speed_limit);
    static int decode_target_speed(struct can_frame* frame, int* target_speed);
    static int decode_acceleration(struct can_frame* frame, double* acceleration);

    static int decode_movement_direction(struct can_frame* frame, int* movement_direction);
    static int decode_trafficlight_light(struct can_frame* frame, int* trafficlight_light);
    static int decode_trafficlight_freq(struct can_frame* frame, int* trafficlight_freq);
    static int decode_passed_distance(struct can_frame* frame, int* passed_distance);
    static int decode_epv_released(struct can_frame* frame, int* epv_state);
    static int decode_epv_key(struct can_frame* frame, int* epv_key);
    static int decode_modules_activity(struct can_frame* frame, ModulesActivity* modulesActivity);
    static int decode_mm_lat_lon(struct can_frame* frame, double* lat, double* lon);
    static int decode_ipd_date(struct can_frame* frame, int* ipd_year, int* ipd_month, int* ipd_day, int* ipd_hours, int* ipd_minutes, int* ipd_seconds);

    static int decode_driving_mode(struct can_frame* frame, int* driving_mode);
    static int decode_vigilance(struct can_frame* frame, int* vigilance);
    static int decode_reg_tape_avl(struct can_frame* frame, int* reg_tape_avl);

    static int decode_pressure_tc_tm(struct can_frame* frame, double* pressure_tc, double* pressure_tm);

    static int decode_ssps_mode(struct can_frame* frame, int* ssps_mode);
    static int decode_traction(struct can_frame* frame, int* in_traction);

    static int decode_is_on_road(struct can_frame* frame, int* is_on_road);

    static int decode_autolock_type(struct can_frame* frame, int* autolock_type);
};


// Если будет больше RMC пакета, то нужно сделать структуру union'ом и ввести enum из пакетов
// http://stackoverflow.com/questions/1127396/structure-constructor-in-c
struct gps_data
{
    double lat;
    double lon;
    int is_reliable;
    int hours;
    int minutes;
    int seconds;
    int subseconds;
    int day;
    int month;
    int year;

    double speed;
};

class nmea
{
public:
    static bool decode_nmea_message(QString message, struct gps_data* gd);
    static void decode_rmc(QString message, struct gps_data* gd);
};


#endif // CANENDEC_H

#endif
