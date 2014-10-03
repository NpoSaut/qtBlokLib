#ifndef PRESSURE_H
#define PRESSURE_H

#include <QString>

// Класс описывает давление
//  удобен для конвертации и преобразования в строку
class Pressure
{
public:
    Pressure()
        : mpa (0), valid (false)
    {}

    void setAtm (float value) { mpa = value * 0.0980665; }
    void setBar (float value) { mpa = value * 0.1; }
    void setMpa (float value) { mpa = value; }
    void setValid (bool val) { valid = val; }

    float getAtm () const { return mpa * 10.19716213; }
    float getBar () const { return mpa * 10; }
    float getMpa () const { return mpa; }
    bool isValid () const { return valid; }

    QString printAtm (bool showUnits) const { return (valid ? QString::number (getAtm(), 'f', 1) : QString("--")) + (showUnits ? QString(" атм") : QString("")); }
    QString printBar (bool showUnits) const { return (valid ? QString::number (getBar(), 'f', 1) : QString("--")) + (showUnits ? QString(" бар") : QString("")); }
    QString printMpa (bool showUnits) const { return (valid ? QString::number (getMpa(), 'f', 2) : QString("--")) + (showUnits ? QString(" МПа") : QString("")); }

    bool operator == (const Pressure &b) const { return getMpa() == b.getMpa() && isValid() == b.isValid(); }
    bool operator != (const Pressure &b) const { return !(*this == b); }

private:
    float mpa;
    bool valid;
};

#endif // PRESSURE_H
