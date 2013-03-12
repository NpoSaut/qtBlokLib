#if defined WITH_CAN || defined WITH_SERIAL

#ifndef ENDECS_H
#define ENDECS_H

#include "iodrvmain.h"

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
    static can_frame encode_sys_key(key_state k_state);
};

class can_decoder
{
public:
    static int decode_speed(struct can_frame* frame, double* speed);
    static int decode_speed_limit(struct can_frame* frame, int* speed_limit);
    static int decode_stop_flag(struct can_frame* frame, int* stop_flag);
    static int decode_movement_direction(struct can_frame* frame, int* movement_direction);
    static int decode_trafficlight_light(struct can_frame* frame, int* trafficlight_light);
    static int decode_trafficlight_freq(struct can_frame* frame, int* trafficlight_freq);
    static int decode_passed_distance(struct can_frame* frame, int* passed_distance);
    static int decode_epv_state(struct can_frame* frame, int* epv_state);
    static int decode_epv_key(struct can_frame* frame, int* epv_key);
    static int decode_mm_lat_lon(struct can_frame* frame, double* lat, double* lon);
    static int decode_ipd_date(struct can_frame* frame, int* ipd_year, int* ipd_month, int* ipd_day, int* ipd_hours, int* ipd_minutes, int* ipd_seconds);
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
};

class nmea
{
public:
    static void decode_nmea_message(QString message, struct gps_data* gd);
    static void decode_rmc(QString message, struct gps_data* gd);
};


#endif // CANENDEC_H

#endif
