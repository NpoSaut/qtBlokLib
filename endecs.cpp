#include <QByteArray>
#include <QStringList>

#include "endecs.h"
#include "cDoodahLib/lowlevel.h"



CanFrame can_encoder::encode_mm_alt_long(double lat, double lon, bool reliability)
{
    CanFrame frame (0x4268, std::vector<unsigned char> (8)); // id: 0x213

    int flat = (int) ( lat * (double)1e8 * 3.14159265359 / (double)180 );
    int flon = ((int) ( lon * (double)1e8 * 3.14159265359 / (double)180 ));
    int flonr = reliability ? ( flon | (1 << 31) ) : ( flon & (~(1 << 31)) );

    frame[1]= flat & 0xFF;
    frame[2] = (flat >> 8) & 0xFF;
    frame[3] = (flat >> 16) & 0xFF;
    frame[4] = (flat >> 24) & 0xFF;

    frame[5] = flonr & 0xFF;
    frame[6] = (flonr >> 8) & 0xFF;
    frame[7] = (flonr >> 16) & 0xFF;
    frame[8] = (flonr >> 24) & 0xFF;

    return frame;
}

CanFrame can_encoder::encode_ipd_date(int year, int month, int day, int hours, int minutes, int seconds)
{
    CanFrame frame (0x18E7, std::vector<unsigned char> (8)); // id: 0x0C7

    frame[1] = (year >> 8) & 0xFF;
    frame[2] = year & 0xFF;
    frame[3] = month;
    frame[4] = day;

    frame[5] = hours;
    frame[6] = minutes;
    frame[7] = seconds;

    return frame;
}

CanFrame can_encoder::encode_disp_state_a()
{
    CanFrame frame (0x51E3, // id: 0x28F
                    std::vector<unsigned char> (3, 0) );

    return frame;
}

CanFrame can_encoder::encode_disp_state_b()
{
    CanFrame frame (0x53E2, // id: 0x29F
                    std::vector<unsigned char> (2, 0) );

    return frame;
}

CanFrame can_encoder::encode_sys_key(key_state k_state, int key_code)
{
    CanFrame frame (0x0C01, std::vector<unsigned char> (1) ); // id: 0x060

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

    frame[1] = key_code + (key_state_flag << 6);

    return frame;
}

CanFrame can_encoder::encode_mm_data(int speed, int milage)
{
    CanFrame frame ( 0x4228, std::vector<unsigned char> (8) ); // id: 0x211

    frame[1] = 0;
    frame[2] = speed & 0xFF;
    frame[3] = 0;
    frame[4] = char(milage/256/256);
    frame[5] = char(milage/256);
    frame[6] = char(milage);
    frame[7] = 0;
    frame[8] = 0;

    return frame;
}

CanFrame can_encoder::encode_ipd_state( double speed, int distance, bool reliable )
{
    CanFrame frame ( 0x1888, std::vector<unsigned char> (8) ); // id: 0x0C4

    distance = abs(distance);

    frame[1] = reliable ? 0 : 1;
    frame[2] = speed != 0 ? 4 : 0; // наличие движения
    frame[3] = speed;
    frame[4] = char(distance/256);
    frame[5] = char(distance);
    frame[6] = char(distance/256/256);
    frame[7] = reliable ? 0 : 1;
    frame[8] = 0;

    return frame;
}

CanFrame can_encoder::encode_autolock_set_message(int autolock_type)
{
    CanFrame frame (0x8D07);
    frame[1] = 0x03;

    switch (autolock_type) {
    case 0: // АБ
        frame[2] = 0x20;
        break;
    case 1: // ПАБ
        frame[2] = 0x29;
        frame[3] = 40;
        break;
    case 2:
        frame[2] = 0x1F;
        frame[3] = 40;
        break;
    default:
        break;
    }

    return frame;
}




// Decode

//======================================

// -1 - incorrect frame ID
// 0 - A != B
// 1 - success

// SAUT_INFO_A
int can_decoder::decode_speed(const CanFrame &frame, double* speed)
{
    if ( frame.getDescriptor () != 0x4668 ) // id: 0x233
        return -1;

    int i0 = ((int)(frame[1]) << 8) + ((int)(frame[2]));
    int i1 = ((i0 & 0x01) << 15) + (i0 >> 1);
    (*speed) = ((double)i1)/128;

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_speed_limit(const CanFrame &frame, int* speed_limit)
{
    if ( frame.getDescriptor () != 0x0A08 ) // id: 0x050
        return -1;

    (*speed_limit) = ( ((int)( frame[4] & 0x80 )) << 8 ) + (int)(frame[2]);

    return 1;
}

// MCO_MODE 0
int can_decoder::decode_autolock_type(const CanFrame &frame, int* autolock_type)
{
    if ( frame.getDescriptor () != 0x0803 ) // id: 0x040
        return -1;

    (*autolock_type) = (int)((frame[1] & 0x0C) >> 2);

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_target_speed(const CanFrame &frame, int* target_speed)
{
    if ( frame.getDescriptor () != 0x0A08 ) // id: 0x050
         return -1;

    (*target_speed) = ( ((int)( frame[4] & 0x40 )) << 8 ) + (int)(frame[3]);

    return 1;
}

// IPD_STATE_A
int can_decoder::decode_acceleration(const CanFrame &frame, double* acceleration)
{
    if ( frame.getDescriptor () != 0x1888 ) // id: 0x0C4
        return -1;

    (*acceleration) = (double)( frame[8] ) / 128;

    return 1;
}


// MCO_LIMITS_A
int can_decoder::decode_epv_released(const CanFrame &frame, int* epv_state)
{
    if ( frame.getDescriptor () != 0x0A48 ) // id:0x052
        return -1;

    (*epv_state) = (int) ( ! ( ( frame[8] >> 7 ) & 0x01 ));

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_epv_key(const CanFrame &frame, int* epv_key)
{
    if ( frame.getDescriptor () != 0x0A08 ) // id: 0x050
        return -1;

    (*epv_key) = (int) ( ( frame[1] >> 6 ) & 0x01 );

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_modules_activity(const CanFrame &frame, ModulesActivity *modulesActivity)
{
    if ( frame.getDescriptor () != 0x0A08 ) // id: 0x050
        return -1;

    (*modulesActivity) = ModulesActivity::loadFromMcoState (
                QByteArray( reinterpret_cast<const char*> (frame.getData().data()), frame.getData().size() ) );

    return 1;
}



// IPD_STATE_A
int can_decoder::decode_movement_direction(const CanFrame &frame, int* movement_direction)
{
    if ( frame.getDescriptor () != 0x1888 ) // id: 0x0C4
        return -1;

    int stop_flag = (int) (( frame[2] >> 2 ) & 0x01 );
    int direction = (int) (( frame[2] >> 7 ) & 0x01 );

    // return -1 = назад, 0 = стоим, +1 = вперёд
    if (stop_flag == 0) // Стоим
    {
        (*movement_direction) = 0;
    }
    else // Едем
    {
        if (direction == 0) // Едем вперёд
        {
            (*movement_direction) = 1;
        }
        else if (direction == 1) // Едем назад
        {
            (*movement_direction) = -1;
        }
    }

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_trafficlight_light(const CanFrame &frame, int* trafficlight_light)
{
    if ( frame.getDescriptor () != 0x0A08 ) // id: 0x050
        return -1;

    // -1 - преобразование для передачи в уровень интерфейса.
    (*trafficlight_light) = (int) ( frame[6] & 0x0F ) - 1;

    return 1;
}

// MP_STATE_A
int can_decoder::decode_trafficlight_freq(const CanFrame &frame, int* trafficlight_freq)
{
    if ( frame.getDescriptor () != 0x11E8 ) // id: 0x08F
        return -1;

    int freq_code = (int) (( frame[5] & 0x30 ) >> 4);

    if (freq_code == 0)
        (*trafficlight_freq) = 50;
    else if (freq_code == 1)
        (*trafficlight_freq) = 75;
    else if (freq_code == 2)
        (*trafficlight_freq) = 0;
    else if (freq_code == 3)
        (*trafficlight_freq) = 25;

    return 1;
}

// IPD_STATE_A
int can_decoder::decode_passed_distance(const CanFrame &frame, int* passed_distance)
{
    if ( frame.getDescriptor () != 0x1888 ) // id: 0x0C4
        return -1;

    struct IntByBytes
    {
        int byte1: 8;
        int byte2: 8;
        int byte3: 8;
        int byte4: 8;
    };

    IntByBytes dist = {frame[5], frame[4], frame[6], (frame[6] & (1 << 7)) ? 0xFF : 0};
    (*passed_distance) = *((int *) &dist);

    return 1;
}

// MY_DEBUG_A
int can_decoder::decode_orig_passed_distance(const CanFrame &frame, int* x)
{
    if ( frame.getDescriptor () != 0x1888 ) // id: 0x0C4
        return -1;

    struct IntByBytes
    {
        int byte1: 8;
        int byte2: 8;
        int byte3: 8;
        int byte4: 8;
    };

#ifdef CPP11
    (*x) = Complex<int32_t> ({frame[5], frame[4], frame[6], (frame[6] & (1 << 7)) ? 0xFF : 0});
#endif

    return 1;
}

// MM_ALT_LONG
int can_decoder::decode_mm_lat_lon(const CanFrame &frame, double* lat, double* lon)
{
    if ( frame.getDescriptor () != 0x4268 ) // id: 0x213
        return -1;

    int lat_i =((int) frame[1]) + (((int) frame[2]) << 8) + (((int) frame[3]) << 16) + (((int) frame[4]) << 24);
    *lat = (double)lat_i * 1e-8 * 180 / 3.14159265359;

    int lon_i =((int) frame[5]) + (((int) frame[6]) << 8) + (((int) frame[7]) << 16) + (((int) (frame[8]) & 0x7F ) << 24);
    *lon = (double)lon_i * 1e-8 * 180 / 3.14159265359;

    return 1;
}

// IPD_DATE
int can_decoder::decode_ipd_date(const CanFrame &frame, int* ipd_year, int* ipd_month, int* ipd_day, int* ipd_hours, int* ipd_minutes, int* ipd_seconds)
{
    if ( frame.getDescriptor () != 0x18E7 ) // id: 0x0C7
        return -1;

    *ipd_year = ( ((int)(frame[1])) << 8 ) + (int)(frame[2]);

    *ipd_month = (int) frame[3];
    *ipd_day = (int) frame[4];

    *ipd_hours = (int) frame[5];
    *ipd_minutes = (int) frame[6];
    *ipd_seconds = (int) frame[7];

    return 1;
}

// MCO_LIMITS_A
int can_decoder::decode_driving_mode(const CanFrame &frame, int* driving_mode)
{
    if ( frame.getDescriptor () != 0x0A48 ) // id: 0x052
        return -1;

    if ( (int) (( frame[2] ) & 0x40 ) )
        (*driving_mode) = 4;
    else
        (*driving_mode) = (int) (( frame[8] ) & 0x03 );

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_vigilance(const CanFrame &frame, int* vigilance)
{
    if ( frame.getDescriptor () != 0x0A08 ) // id: 0x050
        return -1;

    (*vigilance) = (int) (( frame[6] >> 4 ) & 0x01 );

    return 1;
}

// AMR_STATE_I
int can_decoder::decode_reg_tape_avl(const CanFrame &frame, int* reg_tape_avl)
{
    if ( frame.getDescriptor () != 0xCDE1 ) // id: 0x66F
        return -1;

    (*reg_tape_avl) = (int) (( frame[1] ) & 0x01 );

    return 1;
}

// MVD_DD
int can_decoder::decode_pressure_tc_tm(const CanFrame &frame, double* pressure_tc, double* pressure_tm)
{
    if ( frame.getDescriptor () != 0x5D45 ) // id: 0x2EA
        return -1;

    (*pressure_tc) = ((double)(frame[2]))/255;
    (*pressure_tm) = ((double)(frame[3]))/255;

    return 1;
}

// VDS_STATE_A
int can_decoder::decode_ssps_mode(const CanFrame &frame, int* ssps_mode)
{
    if ( frame.getDescriptor () != 0x5C02 ) // id: 0x2E0
        return -1;

    (*ssps_mode) = (int) (( frame[2] ) & 0x01 );

    return 1;
}

// MCO_STATE_A
int can_decoder::decode_traction(const CanFrame &frame, int* in_traction)
{
    if ( frame.getDescriptor () != 0x0A08 ) // id: 0x050
        return -1;

    // Инвертирую для удобства.
    (*in_traction) = (int) ( ! (( frame[1] >> 5 ) & 0x01 ) );

    return 1;
}

// MCO_LIMITS_A
int can_decoder::decode_is_on_road(const CanFrame &frame, int* is_on_road)
{
    if ( frame.getDescriptor () != 0x0A48 ) // id: 0x052
        return -1;

    (*is_on_road) = (int) (( frame[2] ) & 0x40 );

    return 1;
}



bool nmea::decode_nmea_message(QString message, struct gps_data* gd)
{
    if (message.mid(3,3) == "RMC")
    {
        decode_rmc(message, gd);
        return true;
    }
    else return false;
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
    bool IsReliable = fields.at(2).toLower().contains("a");

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

    // Velocityvoid
    //QStringList v_list = fields.at(7).split("."); // И целая, и дробная части могут быть переменной длины.
    //double speed_kmh = ( v_list.at(0).toDouble() + v_list.at(1).toDouble() ) * 1.852;
    double speed_kmh = fields.at(7).toDouble() * 1.852;

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


