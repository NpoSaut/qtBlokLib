#ifndef COOKIES_H
#define COOKIES_H

#include <QObject>


class Cookie : public QObject
{
    Q_OBJECT
public:
    explicit Cookie(QObject *parent = 0);
    int getValue() const;

signals:
    void changed(int newValue);

public slots:
    void setVaule(int value);

private:
//    void loadData(CanFrame frame);
};

class Cookies : public QObject
{
    Q_OBJECT
public:
    explicit Cookies(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // COOKIES_H
