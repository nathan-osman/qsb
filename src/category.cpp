/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

#include "category.hpp"
#include "categorydialog.hpp"
#include "pixmaps.hpp"

Category::Category(Manager *manager, QWidget *parent)
    : QWidget(parent)
    , mManager(manager)
{
    // TODO: smarter width management
    setMinimumWidth(250);
    setMaximumWidth(250);

    QFont font = mLabelName.font();
    font.setPointSize(18);
    mLabelName.setFont(font);
    mLabelName.setText("[Untitled]");

    QPushButton *buttonSettings = new QPushButton;
    buttonSettings->setIcon(getPixmapSettings());
    buttonSettings->setToolTip("Settings");
    connect(buttonSettings, &QAbstractButton::clicked, this, &Category::onSettings);

    QPushButton *buttonRemove = new QPushButton;
    buttonRemove->setIcon(getPixmapRemove());
    buttonRemove->setToolTip("Remove");
    connect(buttonRemove, &QAbstractButton::clicked, this, &Category::remove);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setSpacing(0);
    titleLayout->addWidget(&mLabelName);
    titleLayout->addStretch(1);
    titleLayout->addWidget(buttonSettings);
    titleLayout->addWidget(buttonRemove);

    QFrame *hLine = new QFrame;
    hLine->setContentsMargins(0, 32, 0, 32);
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Sunken);

    mWidgetClips.setLayout(&mLayoutClips);

    QPushButton *buttonNew = new QPushButton("New Clip");
    buttonNew->setIcon(getPixmapNew());
    connect(buttonNew, &QAbstractButton::clicked, this, &Category::onNewClip);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(titleLayout);
    mainLayout->addWidget(hLine);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(&mWidgetClips);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(buttonNew);
    mainLayout->addStretch(1);
}

QJsonObject Category::serialize() const
{
    QJsonObject object;
    object.insert("name", mLabelName.text());

    QJsonArray clips;
    for (int i = 0; i < mLayoutClips.count(); i++) {
        auto clip = dynamic_cast<Clip *>(mLayoutClips.itemAt(i)->widget());
        clips.append(clip->serialize());
    }
    object.insert("clips", clips);

    return object;
}

void Category::deserialize(const QJsonObject &object)
{
    mLabelName.setText(object.value("name").toString());

    const QJsonArray clips = object.value("clips").toArray();
    for (auto i : clips) {
        newClip()->deserialize(i.toObject());
    }
}

void Category::paintEvent(QPaintEvent *)
{
    QStyleOption styleOption;
    styleOption.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
}

void Category::onSettings()
{
    CategoryDialog dialog(mLabelName.text(), this);
    if (dialog.exec() == QDialog::Accepted) {
        mLabelName.setText(dialog.name());
        emit markDirty();
    }
}

void Category::onNewClip()
{
    newClip();
    emit markDirty();
}

Clip *Category::newClip()
{
    Clip *clip = new Clip(mManager, this);
    connect(clip, &Clip::markDirty, this, &Category::markDirty);
    mLayoutClips.addWidget(clip);
    return clip;
}
