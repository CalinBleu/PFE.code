#ifndef GUIMODEL_H
#define GUIMODEL_H

extern "C" {
#include "common.h"
}

#include <QObject>

class GuiModel : public QObject
{
    Q_OBJECT
public:
    explicit GuiModel(QObject *parent = nullptr);

    Q_INVOKABLE void Gui_start();

    Q_INVOKABLE void Gui_askConnexionController(char* password);

    Q_INVOKABLE void Gui_changeModeController(Mode mode);

    Q_INVOKABLE void Gui_userAddedController(User user);

    Q_INVOKABLE void Gui_userModifiedController(User user);

    Q_INVOKABLE void Gui_userRemovedController(char* idTag);

    Q_INVOKABLE void Gui_userSearchedController(char* searchField);



signals:
};

#endif // GUIMODEL_H
