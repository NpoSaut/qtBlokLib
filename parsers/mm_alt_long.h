#ifndef MM_ALT_LONG_H
#define MM_ALT_LONG_H

#include "canblokmessage.h"

// MM_ALT_LONG
// id: 0x213
// len: 8
// desc:0x4268
class MmAltLong : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit MmAltLong(QObject *parent = 0);

    double getLatitude () const { return latitude; }
    double getLongitude () const { return longitude; }
    bool isValid () const { return valid; }
    
signals:
    void latitudeChanged (double lat);
    void longitudeChanged (double lon);
    void validChanged (bool isValid);
    
public slots:
    bool setLatitude (double lat);
    bool setLongitude (double lon);
    bool setValid (bool isValid);

protected:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);
    
    double latitude;
    double longitude;
    bool valid;
};

#endif // MM_ALT_LONG_H
