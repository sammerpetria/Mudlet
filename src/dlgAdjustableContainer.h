#ifndef MUDLET_DLGADJUSTABLECONTAINER_H
#define MUDLET_DLGADJUSTABLECONTAINER_H

// Dialog for creating adjustable containers with miniconsole. right click in new script edition

#include "utils.h"
#include "pre_guard.h"
#include "ui_dlgAdjustableContainer.h"
#include <QDialog>
#include "post_guard.h"

class dlgAdjustableContainer : public QDialog, public Ui::dlgAdjustableContainer
{
    Q_OBJECT

public:
    Q_DISABLE_COPY(dlgAdjustableContainer)
    explicit dlgAdjustableContainer(QWidget* parent = nullptr);

    QString generateCode() const;
};

#endif // MUDLET_DLGADJUSTABLECONTAINER_H
