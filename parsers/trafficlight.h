#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

enum Trafficlight
{
    WHITE = 0,
    RED = 1,
    YELLOW_RED = 2,
    YELLOW = 3,
    GREEN = 4,
    OFF = 5,
    OFF1 = 6,
    OFF2 = 7,
    WHITE_BLINK = 8,
    RED_EN = 9,
    YELLOW_RED_EN = 10,
    FREE_ZONE_1 = 11,
    FREE_ZONE_2 = 12,
    FREE_ZONE_3 = 13,
    FREE_ZONE_4 = 14,
    FREE_ZONE_5 = 15,
    LIGHT_UNKNOWN = -1
};

enum AlsnLight
{
    ALSN_RED = 0,
    ALSN_YELLOW_RED = 1,
    ALSN_YELLOW = 2,
    ALSN_GREEN = 3
};

enum KptType
{
    KPT_UNKNOWN = 0,
    KPT5 = 1,
    KPT7 = 2
};

enum AlsnFrequency
{
    FREQ50_A = 0,
    FREQ75 = 1,
    FREQ50_E = 2,
    FREQ25 = 3
};

#endif // TRAFFICLIGHT_H
