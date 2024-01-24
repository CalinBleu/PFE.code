#ifndef INTERFACEC_H
#define INTERFACEC_H

#include <QObject>

class InterfaceC : public QObject
{
    Q_OBJECT

public:
    explicit InterfaceC(QObject *parent = nullptr);

    Q_INVOKABLE int add(int a, int b);
};

#endif // INTERFACEC_H
