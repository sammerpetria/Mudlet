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

    QString console = lineEdit_console_name->text().trimmed();
    if (console.isEmpty()) {
        console = qsl("console1");
    }

    int fontSize = spinBox_fontsize->value();
    QString color = lineEdit_color->text().trimmed();
    if (color.isEmpty()) {
        color = qsl("black");
    }
    QString scrollBar = checkBox_scrollbar->isChecked() ? qsl("true") : qsl("false");

    QString snippet = qsl("%1 = %1 or Adjustable.Container:new({\n"
                          "  name = \"%1\",\n"
                          "  x = \"-20%\", y = 0,\n"
                          "  width = \"20%\", height = \"20%\",\n"
                          "  autoLoad = true,\n"
                          "})\n\n"
                          "%2 = Geyser.MiniConsole:new({\n"
                          "  name = \"%2\",\n"
                          "  x = 0, y = 4,\n"
                          "  width = \"100%\", height = \"100%-4\",\n"
                          "  fontSize = %3,\n"
                          "  color = \"%4\",\n"
                          "  scrollBar = %5,\n"
                          "}, %1)\n\n")
                              .arg(container, console, QString::number(fontSize), color, scrollBar);

    return snippet;
}
