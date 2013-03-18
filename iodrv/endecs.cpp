#if defined WITH_CAN || defined WITH_SERIAL

#include "endecs.h"


can_frame can_encoder::encode_mm_alt_long(double lat, double lon, bool reliability)
{
    can_frame frame;
    frame.can_id = 0x213;
    frame.can_dlc = 8;

    int flat = (int) ( lat * (double)10e8 * 3.14159265359 / (double)180 );
    int flon = ((int) ( lon * (double)10e8 * 3.14159265359 / (double)180 )) >> 1;
    int flonr = reliability ? ( flon | (1 << 31) ) : ( flon & (~(1 << 31)) );

    frame.data[0] = flat & 0xFF;
    frame.data[1] = (flat >> 8) & 0xFF;
    frame.data[2] = (flat >> 16) & 0xFF;
    frame.data[3] = (flat >> 24) & 0xFF;

    frame.data[4] = flonr & 0xFF;
    frame.data[5] = (flonr >> 8) & 0xFF;
    frame.data[6] = (flonr >> 16) & 0xFF;
    frame.data[7] = (flonr >> 24) & 0xFF;

    return frame;
}

can_frame can_encoder::encode_ipd_date(int year, int month, int day, int hours, int minutes, int seconds)
{
    can_frame frame;
    frame.can_id = 0x0C7;
    frame.can_dlc = 7;

    frame.data[0] = (year >> 8) & 0xFF;
    frame.data[1] = year & 0xFF;
    frame.data[2] = month;
    frame.data[3] = day;

    frame.data[4] = hours;
    frame.data[5] = minutes;
    frame.data[6] = seconds;

    return frame;
}

can_frame can_encoder::encode_disp_state_a()
{
    can_frame frame;
    frame.can_id = 0x28F;
    frame.can_dlc = 3;

    frame.data[0] = 0;
    frame.data[1] = 0;
    frame.data[2] = 0;

    return frame;
}

can_frame can_encoder::encode_disp_state_b()
{
    can_frame frame;
    frame.can_id = 0x29F;
    frame.can_dlc = 2;

    frame.data[0] = 0;
    frame.data[1] = 0;

    return frame;
}

can_frame can_encoder::encode_sys_key(key_state k_state, int key_code)
{
    can_frame frame;
    frame.can_id = 0x060;   // SYS_KEY
    frame.can_dlc = 1;

    int key_state_flag = 0;
    switch(k_state)
    {
        case is_pressed:
            key_state_flag = 1;
            break;
        case is_released:
            key_state_flag = ( 1 << 1 );
            break;
    }

    frame.data[0] = key_code + (key_state_flag << 6);

    return frame;
}

can_frame can_encoder::encode_mm_data(int speed)
{
    can_frame frame;
    frame.can_id = 0x211;
    frame.can_dlc = 8;

    frame.data[0] = 0;
    frame.data[1] = speed & 0b11111111;
    frame.data[2] = 0;
    frame.data[3] = 0;
    frame.data[4] = 0;
    frame.data[5] = 0;
    frame.data[6] = 0;
    frame.data[7] = 0;

    return frame;
}




// Decode

//======================================

// -1 - incorrect frame ID
// 0 - A != B
// 1 - success

// IPD_STATE_A
int can_decoder::decode_speed(struct can_frame* frame, double* speed)
{
    if ((*frame).can_id != 0x0C4) return -1;

    (*speed) =(double)(   ( ((int)( (*frame).data[1] & 0b000000001 )) << 8 ) + (int)((*frame).data[2])   );

    /* SAUT_INFO_A
    double s1 = (double)((*frame).data[0]);
    double s2 = ( (double)( (*frame).data[1] >> 1 ) ) / 128;
    double s3 = (double)( ((*frame).data[1] & 0b00000001 ) == 0 ? 0 : 256 );
    (*speed) = s1 + s2 + s3;*/

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_speed_limit(struct can_frame* frame, int* speed_limit)
{
    if ((*frame).can_id != 0x050) return -1;

    (*speed_limit) = ( ((int)( (*frame).data[3] & 0b10000000 )) << 8 ) + (int)((*frame).data[1]);

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_target_speed(struct can_frame* frame, int* target_speed)
{
    if ((*frame).can_id != 0x050) return -1;

    (*target_speed) = ( ((int)( (*frame).data[3] & 0b01000000 )) << 8 ) + (int)((*frame).data[2]);

    return 1;
}

// IPD_STATE_A
int can_decoder::decode_acceleration(struct can_frame* frame, double* acceleration)
{
    if ((*frame).can_id != 0x0C4) return -1;

    (*acceleration) = (double)( (*frame).data[7] ) / 128;

    return 1;
}


// MCO_STATE_A
int can_decoder::decode_epv_state(struct can_frame* frame, int* epv_state)
{
    if ((*frame).can_id != 0x050) return -1;

    (*epv_state) = (int) ( ( (*frame).data[5] >> 5 ) & 0b00000001 );

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_epv_key(struct can_frame* frame, int* epv_key)
{
    if ((*frame).can_id != 0x050) return -1;

    (*epv_key) = (int) ( ( (*frame).data[0] >> 6 ) & 0b00000001 );

    return 1;
}



// IPD_STATE_A
int can_decoder::decode_movement_direction(struct can_frame* frame, int* movement_direction)
{
    if ((*frame).can_id != 0x0C4) return -1;

    int stop_flag = (int) (( (*frame).data[1] >> 2 ) & 0b00000001 );
    int direction = (int) (( (*frame).data[1] >> 7 ) & 0b00000001 );

    // return -1 = назад, 0 = стоим, +1 = вперёд
    if (stop_flag == 0) // Стоим
    {
        (*movement_direction) = 0;
    }
    else if (direction == 0) // Едем вперёд
    {
        (*movement_direction) = 1;
    }
    else if (direction == 1) // Едем назад
    {
        (*movement_direction) = -1;
    }

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_trafficlight_light(struct can_frame* frame, int* trafficlight_light)
{
    if ((*frame).can_id != 0x050) return -1;

    // -1 - преобразование для передачи в уровень интерфейса.
    (*trafficlight_light) = (int) ( (*frame).data[5] & 0b00001111 ) - 1;

    return 1;
}

// MP_STATE_A
int can_decoder::decode_trafficlight_freq(struct can_frame* frame, int* trafficlight_freq)
{
    if ((*frame).can_id != 0x08F) return -1;

    int freq_code = (int) (( (*frame).data[4] & 0b00110000 ) >> 4);

    if (freq_code == 0)
        (*trafficlight_freq) = 50;
    else if (freq_code == 1)
        (*trafficlight_freq) = 75;
    else if (freq_code == 2)
        (*trafficlight_freq) = 50;
    else if (freq_code == 3)
        (*trafficlight_freq) = 25;

    return 1;
}

// IPD_STATE_A
int can_decoder::decode_passed_distance(struct can_frame* frame, int* passed_distance)
{
    if ((*frame).can_id != 0x0C4) return -1;

    (*passed_distance) = (((int) (*frame).data[5]) << 16) + (((int) (*frame).data[3]) << 8) + ((int) (*frame).data[4]);

    return 1;
}


// MM_ALT_LONG
int can_decoder::decode_mm_lat_lon(struct can_frame* frame, double* lat, double* lon)
{
    if ((*frame).can_id != 0x213) return -1;

    int lat_i =((int) (*frame).data[0]) + (((int) (*frame).data[1]) << 8) + (((int) (*frame).data[2]) << 16) + (((int) (*frame).data[3]) << 24);
    *lat = (double)lat_i * 10e-9 * 180 / 3.14159265359;

    int lon_i =((int) (*frame).data[4]) + (((int) (*frame).data[5]) << 8) + (((int) (*frame).data[6]) << 16) + (((int) ((*frame).data[7]) & 0b01111111 ) << 24);
    *lon = (double)lon_i * 10e-9 * 180 / 3.14159265359;

    return 1;
}

// IPD_DATE
int can_decoder::decode_ipd_date(struct can_frame* frame, int* ipd_year, int* ipd_month, int* ipd_day, int* ipd_hours, int* ipd_minutes, int* ipd_seconds)
{
    if ((*frame).can_id != 0x0C7) return -1;

    *ipd_year = ( ((int)((*frame).data[0])) << 8 ) + (int)((*frame).data[1]);

    *ipd_month = (int) (*frame).data[2];
    *ipd_day = (int) (*frame).data[3];

    *ipd_hours = (int) (*frame).data[4];
    *ipd_minutes = (int) (*frame).data[5];
    *ipd_seconds = (int) (*frame).data[6];

    return 1;
}

// MCO_LIMITS_A
int can_decoder::decode_driving_mode(struct can_frame* frame, int* driving_mode)
{
    if ((*frame).can_id != 0x052) return -1;

    (*driving_mode) = (int) (( (*frame).data[7] ) & 0b00000011 );

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_vigilance(struct can_frame* frame, int* vigilance)
{
    if ((*frame).can_id != 0x050) return -1;

    (*vigilance) = (int) (( (*frame).data[5] >> 4 ) & 0b00000001 );

    return 1;
}

// AMR_STATE_I
int can_decoder::decode_reg_tape_avl(struct can_frame* frame, int* reg_tape_avl)
{
    if ((*frame).can_id != 0x66F) return -1;

    (*reg_tape_avl) = (int) (( (*frame).data[0] ) & 0b00000001 );

    return 1;
}

// MVD_DD
int can_decoder::decode_pressure_tc_tm(struct can_frame* frame, double* pressure_tc, double* pressure_tm)
{
    if ((*frame).can_id != 0x2EA) return -1;

    (*pressure_tc) = ((double)((*frame).data[1]))/255;
    (*pressure_tm) = ((double)((*frame).data[2]))/255;

    return 1;
}

// VDS_STATE_A
int can_decoder::decode_ssps_mode(struct can_frame* frame, int* ssps_mode)
{
    if ((*frame).can_id != 0x2E0) return -1;

    (*ssps_mode) = (int) (( (*frame).data[1] ) & 0b00000001 );

    return 1;
}



void nmea::decode_nmea_message(QString message, struct gps_data* gd)
{
    if (message.mid(3,3) == "RMC")
    {
        decode_rmc(message, gd);
    }
}

// $GPRMC,024607.000,V,5651.27857,N,06035.91777,E,0.0,0.0,241212,,,N*70
//-$GхRMC,HHMMSS.SS,A,BBBB.BBBB,a,LLLLL.LLLL,a,v.v,z.z,DDMMYY,x.x,a,b* hh
/* Формат сообщения RMC
 0|  $GNRMC,
 1|  HHMMSS.SSSS,
 2|  A,
 3|  BBBB.BBBB,
 4|  a,
 5|  LLLLL.LLLL,
 6|  a,
 7|  v.v,
 8|  z.z,
 9|  DDMMYY,
10|  x.x,
11|  a,
12|  b* hh<CR><LF> */
void nmea::decode_rmc(QString message, struct gps_data* gd)
{
    QStringList fields = message.split(",");

    // Datetime
    int dth = fields.at(1).mid(0,2).toInt();
    int dtm = fields.at(1).mid(2,2).toInt();
    int dts = fields.at(1).mid(4,2).toInt();
    int dtss = fields.at(1).mid(7,-1).toInt();
    int dtd = fields.at(9).mid(0,2).toInt();
    int dtmn = fields.at(9).mid(2,2).toInt();
    int dty = ("20" + fields.at(9).mid(4,2)).toInt();

    // Reliability
    bool IsReliable = false;
    if (fields.at(2) == "A")
        IsReliable = true;
    else if (fields.at(2) == "V")
        IsReliable = false;

    // Lattitude, degrees
    double latd = fields.at(3).mid(0,2).toDouble();
    double latmmm = fields.at(3).mid(2,-1).toDouble();
    double lat = latd + latmmm/60;
    if (fields.at(4) == "S")
        lat = -lat;

    // Longitude, degrees
    double lond = fields.at(5).mid(0,3).toDouble();
    double lonmmm = fields.at(5).mid(3,-1).toDouble();
    double lon = lond + lonmmm/60;
    if (fields.at(6) == "W")
        lon = -lon;

    // Velocity
    QStringList v_list = fields.at(7).split("."); // И целая, и дробная части могут быть переменной длины.
    double speed_kmh = ( v_list.at(0).toDouble() + v_list.at(1).toDouble() ) * 1.852;

    gd->lat = lat;
    gd->lon = lon;
    gd->is_reliable = IsReliable;
    gd->hours = dth;
    gd->minutes = dtm;
    gd->seconds = dts;
    gd->subseconds = dtss;
    gd->day = dtd;
    gd->month = dtmn;
    gd->year = dty;
    gd->speed = speed_kmh;
}

#endif
