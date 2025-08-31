#ifndef MUDLET_DLGADJUSTABLECONTAINER_H
#define MUDLET_DLGADJUSTABLECONTAINER_H

/***************************************************************************
 *   Copyright (C) 2024 by OpenAI                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

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
