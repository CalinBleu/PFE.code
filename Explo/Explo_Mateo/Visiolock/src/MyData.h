// MyData.h
#ifndef MYDATA_H
#define MYDATA_H

#include <QObject>

class MyData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString myString READ getMyString WRITE setMyString NOTIFY myStringChanged)

public:
    MyData(QObject *parent = nullptr) : QObject(parent) {}

    QString getMyString() const {
        return m_myString;
    }

    void setMyString(const QString &value) {
        if (value != m_myString) {
            m_myString = value;
            emit myStringChanged();
        }
    }

signals:
    void myStringChanged();

private:
    QString m_myString;
};

#endif // MYDATA_H
