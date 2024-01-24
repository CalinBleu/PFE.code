#include "interfacec.h"
extern "C" {
#include "test.h"
}

InterfaceC::InterfaceC(QObject *parent) : QObject(parent) {}

int InterfaceC::add(int a, int b)
{
    return ::add(a, b);
}

