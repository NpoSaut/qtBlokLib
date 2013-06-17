#include "lowlevel.h"
#include "cookies.h"


Cookie::Cookie(QObject *parent)
{
}

Cookie::Cookie(int index)
    : index (index)
{ }

int Cookie::getValue() const
{
    return value;
}

Cookies::Cookies(QObject *parent) :
    QObject(parent)
{
}

void Cookie::loadData(const CanFrame &frame)
{
    loadDataWithControl (frame);
}

bool Cookie::loadDataWithControl(const CanFrame &frame)
{
    if ( frame.getId () == 0x6265 || frame.getId () == 0x6285 ) // SYS_DATA
    {
        auto byte = frame.getData ();
        if ( byte[0] == index )
        {
            value = Complex<uint32_t> {byte[0], byte[1], byte[2], byte[3]} ;
            return true;
        }
    }
    return false;
}
