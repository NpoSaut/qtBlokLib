#ifndef IODRVMAIN_H
#define IODRVMAIN_H


#include <QMainWindow>
#include <QObject>
#include <QtConcurrentRun>
#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialportinfo.h>
#include <QTimer>
#include <QElapsedTimer>
#include <QList>


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <string.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <errno.h>


#endif // IODRVMAIN_H
