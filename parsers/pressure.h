#ifndef PRESSURE_H
#define PRESSURE_H

#include <QString>

// Класс описывает давление
//  удобен для конвертации и преобразования в строку
class Pressure
{
public:
    Pressure()
        : mpa (0), validity (false)
    {}

    void setAtm (float value) { mpa = value * 0.0980665; }
    void setBar (float value) { mpa = value * 0.1; }
    void setMpa (float value) { mpa = value; }
    void setValidity (bool val) { validity = val; }

    float getAtm () const { return mpa * 10.19716213; }
    float getBar () const { return mpa * 10; }
    float getMpa () const { return mpa; }
    bool getValidity () const { return validity; }

    QString printAtm () const { return (validity ? QString::number (getAtm(), 'f', 1) : QString("--")) + QString(" атм") ; }
    QString printBar () const { return (validity ? QString::number (getBar(), 'f', 1) : QString("--")) + QString(" бар") ; }
    QString pringMpa () const { return (validity ? QString::number (getMpa(), 'f', 2) : QString("--")) + QString(" МПа") ; }

    bool operator == (const Pressure &b) const { return getMpa() == b.getMpa() && getValidity() == b.getValidity(); }
    bool operator != (const Pressure &b) const { return !(*this == b); }

private:
    float mpa;
    bool validity;
};

#endif // PRESSURE_H
