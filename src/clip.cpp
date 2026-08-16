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

#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QStyleOption>

#include "clip.hpp"
#include "clipdialog.hpp"
#include "pixmaps.hpp"

Clip::Clip(Manager *manager, QWidget *parent)
    : QWidget(parent)
    , mManager(manager)
    , mPlayer(nullptr)
    , mVolume(100)
{
    mButtonPlayStop.setIcon(getPixmapPlay());
    mButtonPlayStop.setToolTip("Play / Stop");
    connect(&mButtonPlayStop, &QAbstractButton::clicked, this, &Clip::onPlayStop);

    mLabelName.setText("[Untitled]");

    QPushButton *buttonSettings = new QPushButton;
    buttonSettings->setIcon(getPixmapSettings());
    buttonSettings->setToolTip("Settings");
    connect(buttonSettings, &QAbstractButton::clicked, this, &Clip::onSettings);

    QHBoxLayout *hboxLayout = new QHBoxLayout(this);
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    hboxLayout->addWidget(&mButtonPlayStop);
    hboxLayout->addWidget(&mLabelName);
    hboxLayout->addStretch(1);
    hboxLayout->addWidget(buttonSettings);
    setLayout(hboxLayout);
}

QJsonObject Clip::serialize() const
{
    QJsonObject object;
    object.insert("name", mLabelName.text());
    object.insert("filename", mFilename);
    object.insert("volume", mVolume);
    return object;
}

void Clip::deserialize(const QJsonObject &object)
{
    mLabelName.setText(object.value("name").toString());
    mFilename = object.value("filename").toString();
    mVolume = object.value("volume").toInt();
}

void Clip::paintEvent(QPaintEvent *)
{
    QStyleOption styleOption;
    styleOption.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
}

void Clip::onPlayStop()
{
    if (mPlayer) {
        mPlayer->stop();
    } else {
        mPlayer = mManager->queue(mFilename, mVolume);
        connect(mPlayer, &Player::error, this, &Clip::onPlayerFinished);
        connect(mPlayer, &Player::finished, this, &Clip::onPlayerFinished);
        mPlayer->play();
        mButtonPlayStop.setIcon(getPixmapStop());
    }
}

void Clip::onSettings()
{
    ClipDialog dialog(mLabelName.text(), mFilename, mVolume, this);
    if (dialog.exec() == QDialog::Accepted) {
        mLabelName.setText(dialog.name());
        mFilename = dialog.filename();
        mVolume = dialog.volume();
        emit markDirty();
    }
}

void Clip::onPlayerFinished()
{
    disconnect(mPlayer, &Player::error, this, &Clip::onPlayerFinished);
    disconnect(mPlayer, &Player::finished, this, &Clip::onPlayerFinished);
    mPlayer = nullptr;
    mButtonPlayStop.setIcon(getPixmapPlay());
}