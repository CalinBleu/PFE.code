#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

extern "C" {
#include "src/gui/gui.h"
}
#include <QObject>

class GuiController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool screenStatus READ getScreenStatus WRITE GuiController_activateScreen NOTIFY screenStatusChanged)
    Q_PROPERTY(bool passResult READ getPassResult WRITE GuiController_passResult NOTIFY passResultChanged)
    Q_PROPERTY(QString newUserTag READ getNewUserTag WRITE GuiController_displayNewUserTag NOTIFY newUserTagChanged)



public:

    GuiController(QObject *parent = nullptr) : QObject(parent) {}

    bool getScreenStatus() {
        return screenStatus;
    }

    void GuiController_activateScreen(bool value) {
        screenStatus = value;
        emit screenStatusChanged();
    }


    bool getPassResult()
    {
        return passResult;
    }

    void GuiController_passResult(bool result)
    {
        passResult = result;
        emit passResultChanged();
    }


    QString getNewUserTag()
    {
        return newUserTag;
    }

    void GuiController_displayNewUserTag(QString &tag)
    {
        if(newUserTag != tag){
            newUserTag = tag;
            emit newUserTagChanged();
        }
    }

signals:
    void screenStatusChanged();
    void passResultChanged();
    void newUserTagChanged();

private:
    bool screenStatus;
    bool passResult;
    QString newUserTag;
};

#endif // GUICONTROLLER_H
