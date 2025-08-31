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

#include "dlgAdjustableContainer.h"

#include "pre_guard.h"
#include <QString>
#include "post_guard.h"

dlgAdjustableContainer::dlgAdjustableContainer(QWidget* parent) : QDialog(parent)
{
    setupUi(this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString dlgAdjustableContainer::generateCode() const
{
    QString container = lineEdit_container_name->text().trimmed();
    if (container.isEmpty()) {
        container = qsl("myContainer");
    }

    QString firstConsole = lineEdit_console1_name->text().trimmed();
    if (firstConsole.isEmpty()) {
        firstConsole = qsl("console1");
    }

    QString secondConsole = lineEdit_console2_name->text().trimmed();
    if (secondConsole.isEmpty()) {
        secondConsole = qsl("console2");
    }

    const bool vertical = comboBox_orientation->currentIndex() == 1;

    QString snippet = qsl("%1 = Adjustable.Container:new({\n"
                          "  name = \"%1\",\n"
                          "  x = 0, y = 0,\n"
                          "  width = \"100%\", height = \"100%\"\n"
                          "})\n")
                              .arg(container);

    if (vertical) {
        snippet += qsl("%2 = Geyser.MiniConsole:new({name=\"%2\", x=\"0%\", y=\"0%\", width=\"100%\", height=\"50%\"}, %1)\n"
                       "%3 = Geyser.MiniConsole:new({name=\"%3\", x=\"0%\", y=\"50%\", width=\"100%\", height=\"50%\"}, %1)\n")
                           .arg(container, firstConsole, secondConsole);
    } else {
        snippet += qsl("%2 = Geyser.MiniConsole:new({name=\"%2\", x=\"0%\", y=\"0%\", width=\"50%\", height=\"100%\"}, %1)\n"
                       "%3 = Geyser.MiniConsole:new({name=\"%3\", x=\"50%\", y=\"0%\", width=\"50%\", height=\"100%\"}, %1)\n")
                           .arg(container, firstConsole, secondConsole);
    }

    return snippet;
}
