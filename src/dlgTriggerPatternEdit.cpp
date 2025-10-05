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
#include <QColor>
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

    if (!baseColor.isValid() || !textColor.isValid() || baseColor.lightness() >= 128) {
        resetThemePalette();
        return;
    }

    QPalette framePalette = mDefaultPalette;
    framePalette.setColor(QPalette::Window, baseColor);
    framePalette.setColor(QPalette::Base, baseColor);
    framePalette.setColor(QPalette::AlternateBase, baseColor);
    framePalette.setColor(QPalette::WindowText, textColor);
    framePalette.setColor(QPalette::Text, textColor);
    framePalette.setColor(QPalette::Button, baseColor);
    framePalette.setColor(QPalette::ButtonText, textColor);

    setPalette(framePalette);
    setAutoFillBackground(true);

    auto applyToWidget = [&](QWidget* widget, const QPalette& defaultPalette) {
        QPalette widgetPalette = defaultPalette;
        widgetPalette.setColor(QPalette::Window, baseColor);
        widgetPalette.setColor(QPalette::Base, baseColor);
        widgetPalette.setColor(QPalette::AlternateBase, baseColor);
        widgetPalette.setColor(QPalette::Text, textColor);
        widgetPalette.setColor(QPalette::WindowText, textColor);
        widgetPalette.setColor(QPalette::Button, baseColor);
        widgetPalette.setColor(QPalette::ButtonText, textColor);
        widget->setPalette(widgetPalette);
    };

    applyToWidget(label_patternNumber, mDefaultPatternNumberPalette);
    applyToWidget(label_prompt, mDefaultPromptPalette);
    applyToWidget(comboBox_patternType, mDefaultComboPalette);
    applyToWidget(spinBox_lineSpacer, mDefaultSpinPalette);
    applyToWidget(pushButton_fgColor, mDefaultForegroundButtonPalette);
    applyToWidget(pushButton_bgColor, mDefaultBackgroundButtonPalette);
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
}
