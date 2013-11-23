#ifndef ENDECS_H
#define ENDECS_H

#include <QString>

#include <qtCanLib/canframe.h>
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
    static CanFrame encode_mm_alt_long(double lat, double lon, bool reliability);
    static CanFrame encode_ipd_date(int year, int month, int day, int hours, int minutes, int seconds);
    static CanFrame encode_sys_key(key_state k_state, int key_code);
    static CanFrame encode_mm_data(int speed, int milage);
    static CanFrame encode_ipd_state( double speed, int distance, bool reliable );
    static CanFrame encode_autolock_set_message (int autolock_type, int speed);
};

class can_decoder
{
public:
    static int decode_speed(const CanFrame &frame, double* speed);
    static int decode_speed_limit(const CanFrame &frame, int* speed_limit);
    static int decode_target_speed(const CanFrame &frame, int* target_speed);
    static int decode_acceleration(const CanFrame &frame, double* acceleration);

    static int decode_movement_direction(const CanFrame &frame, int* movement_direction);
    static int decode_passed_distance(const CanFrame &frame, int* passed_distance);
    static int decode_orig_passed_distance(const CanFrame &frame, int* x);
    static int decode_epv_released(const CanFrame &frame, int* epv_state);
    static int decode_epv_key(const CanFrame &frame, int* epv_key);
    static int decode_modules_activity(const CanFrame &frame, ModulesActivity* modulesActivity);
    static int decode_mm_lat_lon(const CanFrame &frame, double* lat, double* lon);
    static int decode_ipd_date(const CanFrame &frame, int* ipd_year, int* ipd_month, int* ipd_day, int* ipd_hours, int* ipd_minutes, int* ipd_seconds);

    static int decode_driving_mode(const CanFrame &frame, int* driving_mode);
    static int decode_vigilance(const CanFrame &frame, int* vigilance);
    static int decode_reg_tape_avl(const CanFrame &frame, int* reg_tape_avl);

    static int decode_autolock_type(const CanFrame &frame, int* autolock_type);
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
