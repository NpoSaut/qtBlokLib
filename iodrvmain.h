#ifndef IODRVMAIN_H
#define IODRVMAIN_H


#include <QObject>
#include <QtConcurrentRun>
#include <QTimer>
#include <QElapsedTimer>
#include <QList>

#ifdef WITH_SERIALPORT
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>
#endif

#include <string.h>

//#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <errno.h>


#endif // IODRVMAIN_H
