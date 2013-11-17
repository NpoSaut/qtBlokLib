#ifndef MM_COORD_H
#define MM_COORD_H

#include <QObject>

#include "canblokmessage.h"

// MM_COORD
// id: 0x605
// len: 3
// desc:0xC0A3
class MmCoord : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit MmCoord (QObject *parent = 0);

    int getRailWayCoordinate () const { return railWayCoordinate; }

    CanFrame encode () const;

signals:
    void railWayCoordinateChanged (int coordinate);
    void whateverChanged ();
    void messageReceived ();

public slots:
    void setRailWayCoordinate (int coordiante);

private slots:
    void processCanMessage (CanFrame frame);

private:
    int railWayCoordinate;
};

#endif // MM_COORD_H
