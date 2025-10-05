/***************************************************************************
 *   Copyright (C) 2008-2009 by Heiko Koehn - KoehnHeiko@googlemail.com    *
 *   Copyright (C) 2014 by Ahmed Charles - acharles@outlook.com            *
 *   Copyright (C) 2019, 2022 by Stephen Lyons - slysven@virginmedia.com   *
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


#include "dlgTriggerPatternEdit.h"

#include "pre_guard.h"
#include <QAbstractButton>
#include <QAbstractItemView>
#include <QAbstractSpinBox>
#include <QColor>
#include <QComboBox>
#include <QLineEdit>
#include <QPalette>
#include <QWidget>
#include "post_guard.h"

dlgTriggerPatternEdit::dlgTriggerPatternEdit(QWidget* pParentWidget)
: QWidget(pParentWidget)
{
    // init generated dialog
    setupUi(this);

    mDefaultPalette = palette();
    mDefaultPatternNumberPalette = label_patternNumber->palette();
    mDefaultPromptPalette = label_prompt->palette();
    mDefaultComboPalette = comboBox_patternType->palette();
    mDefaultSpinPalette = spinBox_lineSpacer->palette();
    mDefaultForegroundButtonPalette = pushButton_fgColor->palette();
    mDefaultBackgroundButtonPalette = pushButton_bgColor->palette();

    // delay the connection so the pattern type is available for the slot
    connect(comboBox_patternType, qOverload<int>(&QComboBox::currentIndexChanged), this, &dlgTriggerPatternEdit::slot_triggerTypeComboBoxChanged, Qt::QueuedConnection);
}

void dlgTriggerPatternEdit::slot_triggerTypeComboBoxChanged(const int index)
{
    label_colorIcon->setPixmap(comboBox_patternType->itemIcon(index).pixmap(15, 15));

}

void dlgTriggerPatternEdit::applyThemePalette(const QPalette& editorPalette)
{
    const QColor baseColor = editorPalette.color(QPalette::Base);
    const QColor textColor = editorPalette.color(QPalette::Text);

    if (!baseColor.isValid() || !textColor.isValid()) {
        resetThemePalette();
        return;
    }

    if (baseColor.lightness() >= textColor.lightness()) {
        resetThemePalette();
        return;
    }

    auto makePalette = [&](QPalette palette) {
        const QColor alternateBase = editorPalette.color(QPalette::AlternateBase).isValid() ? editorPalette.color(QPalette::AlternateBase) : baseColor;
        const QColor buttonColor = editorPalette.color(QPalette::Button).isValid() ? editorPalette.color(QPalette::Button) : baseColor;
        QColor placeholderColor = editorPalette.color(QPalette::PlaceholderText);
        if (!placeholderColor.isValid()) {
            placeholderColor = textColor;
            placeholderColor.setAlphaF(0.6);
        }

        palette.setColor(QPalette::Window, baseColor);
        palette.setColor(QPalette::Base, baseColor);
        palette.setColor(QPalette::AlternateBase, alternateBase);
        palette.setColor(QPalette::Button, buttonColor);
        palette.setColor(QPalette::WindowText, textColor);
        palette.setColor(QPalette::Text, textColor);
        palette.setColor(QPalette::ButtonText, textColor);
        palette.setColor(QPalette::PlaceholderText, placeholderColor);

        palette.setColor(QPalette::Disabled, QPalette::WindowText, textColor);
        palette.setColor(QPalette::Disabled, QPalette::Text, textColor);
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, textColor);
        palette.setColor(QPalette::Disabled, QPalette::Base, baseColor);
        palette.setColor(QPalette::Disabled, QPalette::Button, buttonColor);
        palette.setColor(QPalette::Disabled, QPalette::AlternateBase, alternateBase);

        return palette;
    };

    const QPalette darkPalette = makePalette(editorPalette);

    setPalette(darkPalette);
    setAutoFillBackground(true);

    auto applyToWidget = [&](QWidget* widget) {
        QPalette widgetPalette = makePalette(editorPalette);
        widget->setPalette(widgetPalette);

        if (auto* spinBox = qobject_cast<QAbstractSpinBox*>(widget)) {
            if (auto* lineEdit = spinBox->findChild<QLineEdit*>()) {
                lineEdit->setPalette(widgetPalette);
            }
        }

        if (auto* comboBox = qobject_cast<QComboBox*>(widget)) {
            if (auto* view = comboBox->view()) {
                view->setPalette(widgetPalette);
            }
        }

        if (auto* button = qobject_cast<QAbstractButton*>(widget)) {
            button->setAutoFillBackground(true);
        }
    };

    applyToWidget(label_patternNumber);
    applyToWidget(label_prompt);
    applyToWidget(comboBox_patternType);
    applyToWidget(spinBox_lineSpacer);
    applyToWidget(pushButton_fgColor);
    applyToWidget(pushButton_bgColor);
}

void dlgTriggerPatternEdit::resetThemePalette()
{
    setAutoFillBackground(false);
    setPalette(mDefaultPalette);

    label_patternNumber->setPalette(mDefaultPatternNumberPalette);
    label_prompt->setPalette(mDefaultPromptPalette);
    comboBox_patternType->setPalette(mDefaultComboPalette);
    spinBox_lineSpacer->setPalette(mDefaultSpinPalette);
    pushButton_fgColor->setPalette(mDefaultForegroundButtonPalette);
    pushButton_bgColor->setPalette(mDefaultBackgroundButtonPalette);

    if (auto* spinBoxLineEdit = spinBox_lineSpacer->findChild<QLineEdit*>()) {
        spinBoxLineEdit->setPalette(mDefaultSpinPalette);
    }

    if (auto* comboBoxView = comboBox_patternType->view()) {
        comboBoxView->setPalette(mDefaultComboPalette);
    }

    pushButton_fgColor->setAutoFillBackground(false);
    pushButton_bgColor->setAutoFillBackground(false);
}
