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

can_frame can_encoder::encode_sys_key(key_state k_state)
{
    can_frame frame;
    frame.can_id = 0x060;
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

    int key_code = 0x1C;

    frame.data[0] = key_code + (key_state_flag << 6);

    return frame;
}


// Decode

//======================================

// -1 - incorrect frame ID
// 0 - A != B
// 1 - success

// SAUT_INFO_A
int can_decoder::decode_speed(struct can_frame* frame, double* speed)
{
    if ((*frame).can_id != 0x233) return -1;

    double s1 = (double)((*frame).data[0]);
    double s2 = ( (double)( (*frame).data[1] >> 1 ) ) / 128;
    double s3 = (double)( ((*frame).data[1] & 0b00000001 ) == 0 ? 0 : 256 );

    (*speed) = s1 + s2 + s3;
    return 1;
}

// MCO_STATE_A
int can_decoder::decode_speed_limit(struct can_frame* frame, int* speed_limit)
{
    if ((*frame).can_id != 0x050) return -1;

    (*speed_limit) = ( ((int)( (*frame).data[3] & 0b10000000 )) << 8 ) + (int)((*frame).data[1]);

    return 1;
}

// SAUT_INFO_A
int can_decoder::decode_stop_flag(struct can_frame* frame, int* stop_flag)
{
    if ((*frame).can_id != 0x233) return -1;

    (*stop_flag) = (int) (( (*frame).data[7] >> 2 ) & 0b00000001 );

    return 1;
}

// IPD_STATE_A
int can_decoder::decode_movement_direction(struct can_frame* frame, int* movement_direction)
{
    if ((*frame).can_id != 0x0C4) return -1;

    (*movement_direction) = (int) (( (*frame).data[1] >> 7 ) & 0b00000001 );

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_trafficlight_light(struct can_frame* frame, int* trafficlight_light)
{
    if ((*frame).can_id != 0x050) return -1;

    (*trafficlight_light) = (int) ( (*frame).data[5] & 0b00001111 );

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
int can_decoder::decode_ipd_datetime(struct can_frame* frame, int* ipd_hours, int* ipd_min, int* ipd_sec)
{
    if ((*frame).can_id != 0x0C7) return -1;

    *ipd_hours = (int) (*frame).data[4];
    *ipd_min = (int) (*frame).data[5];
    *ipd_sec = (int) (*frame).data[6];

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
}

