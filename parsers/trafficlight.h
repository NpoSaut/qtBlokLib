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
    YELLOW_EN = 11,
    GREEN_1_EN = 12,
    GREEN_2_EN = 13,
    GREEN_3_EN = 14,
    GREEN_4_EN = 15
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
