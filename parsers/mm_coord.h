#ifndef MM_COORD_H
#define MM_COORD_H

#include <QObject>

#include "canblokmessage.h"

class MmCoord : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit MmCoord (QObject *parent = 0);

    int getRailWayCoordinate () const { return railWayCoordinate; }

signals:
    void railWayCoordinateChanged (int coordinate);
    void whateverChanged ();

private slots:
    void getCanMessage (CanFrame frame);

private:
    int railWayCoordinate;
};

#endif // MM_COORD_H
