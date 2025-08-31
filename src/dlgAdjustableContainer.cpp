#include "dlgAdjustableContainer.h"
#include "pre_guard.h"
#include <QString>
#include "post_guard.h"

// Dialog for creating adjustable containers with miniconsole. right click in new script edition

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


    QString titleText = lineEdit_titleText->text().trimmed();
    if (titleText.isEmpty()) {
        titleText = container;
    }

    QString console = lineEdit_console_name->text().trimmed();
    if (console.isEmpty()) {
        console = qsl("console1");
    }

    QString x = lineEdit_x->text().trimmed();
    if (x.isEmpty()) {
        x = qsl("-20%");
    }

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

    QString scrollBarEnabled = checkBox_scrollbar->isChecked() ? qsl("true") : qsl("false");

    QString snippet = qsl("%1 = %1 or Adjustable.Container:new({\n"
                          "  name = \"%1\",\n"
                          "  titleText = \"%2\",\n"
                          "  x = \"%3\", y = %4,\n"
                          "  width = \"%5\", height = \"%6\",\n"
                          "  autoLoad = true,\n"
                          "})\n\n"
                          "%7 = %7 or Geyser.MiniConsole:new({\n"
                          "  name = \"%7\",\n"
                          "  x = 0, y = 4,\n"
                          "  width = \"100%\", height = \"100%-4\",\n"
                          "  autoWrap = true,\n"
                          "  fontSize = %8,\n"
                          "  color = \"%9\",\n"
                          "  scrollBar = %10,\n"
                          "}, %1)\n\n");
    snippet = snippet.arg(container).arg(titleText).arg(x).arg(yOutput).arg(width).arg(height).arg(console).arg(QString::number(fontSize)).arg(color).arg(scrollBarEnabled);
    return snippet;
}
