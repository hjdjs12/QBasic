#ifndef CUSTOM_EXCEPTION_H
#define CUSTOM_EXCEPTION_H

#include <QObject>

class Custom_exception
{
public:
    Custom_exception(const QString input):message(input){}
    const QString what() {return message;}
    QString message;
};

#endif // CUSTOM_EXCEPTION_H
