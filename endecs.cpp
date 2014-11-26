#include <QByteArray>
#include <QStringList>

#include "endecs.h"
#include "cDoodahLib/lowlevel.h"

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




