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


    QString x = lineEdit_x->text().trimmed();
    if (x.isEmpty()) {
        x = qsl("-20%");
    }
    QString scrollBar = checkBox_scrollbar->isChecked() ? qsl("true") : qsl("false");

    QString y = lineEdit_y->text().trimmed();
    if (y.isEmpty()) {
        y = qsl("0");
    }
    QString yOutput = y.contains('%') ? qsl("\"%1\"").arg(y) : y;

    QString width = lineEdit_width->text().trimmed();
    if (width.isEmpty()) {
        width = qsl("20%");
    }

    QString height = lineEdit_height->text().trimmed();
    if (height.isEmpty()) {
        height = qsl("20%");
    }

    int fontSize = spinBox_fontsize->value();
    QString color = lineEdit_color->text().trimmed();
    if (color.isEmpty()) {
        color = qsl("black");
    }
    QString scrollBar = checkBox_scrollbar->isChecked() ? qsl("true") : qsl("false");

    QString snippet = qsl("%1 = %1 or Adjustable.Container:new({\n"
                          "  name = \"%1\",\n"
                          "  x = \"%2\", y = %3,\n"
                          "  width = \"%4\", height = \"%5\",\n"
                          "  autoLoad = true,\n"
                          "})\n\n"
                          "%6 = %6 or Geyser.MiniConsole:new({\n"
                          "  name = \"%6\",\n"
                          "  x = 0, y = 4,\n"
                          "  width = \"100%\", height = \"100%-4\",\n"
                          "  autoWrap = true,\n"
                          "  fontSize = %7,\n"
                          "  color = \"%8\",\n"
                          "  scrollBar = %9,\n"
                          "}, %1)\n\n")
                              .arg(container, x, yOutput, width, height, console, QString::number(fontSize), color, scrollBar);
    return snippet;
}
